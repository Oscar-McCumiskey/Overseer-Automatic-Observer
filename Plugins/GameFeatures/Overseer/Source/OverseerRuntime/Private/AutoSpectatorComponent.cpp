// Fill out your copyright notice in the Description page of Project Settings.

#include "AutoSpectatorComponent.h"

#include "ComponentUtils.h"
#include "SpectatePriorityTracker.h"
#include "Slate/SGameLayerManager.h"

// Sets default values for this component's properties
UAutoSpectatorComponent::UAutoSpectatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ..

	CurrentSpecTarget = nullptr;
	World = nullptr;
}


// Called when the game starts
void UAutoSpectatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UAutoSpectatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (cameraTimer <= 0)
	{
		FindNewTarget = true;
		
		if (HasChangedSpectateTarget)
		{
			// Prevent rapid switching
			cameraTimer = CameraSwitchTime;
			HasChangedSpectateTarget = false;
		}
		else
		{
			// Spectator spectate frequency
			cameraTimer = 0.1f;
		}
	}
	cameraTimer -= DeltaTime;

	// Heat map timer
	if (heatMapTimer > 0)
	{
		heatMapTimer-= DeltaTime;
	}
	else
	{
		// Update heat map
		heatMapTimer = 1 / heatMapUpdateRate;
		
		if (HeatMapActive)
			PlayerHeatMapPriority();
	}
}

// Find player pawn with the highest priority in priority map
AController* UAutoSpectatorComponent::FindHighestPriorityPlayer()
{
	AController* highestPriorityPlayer = nullptr;
	int highestPriority = -1;
	
	for (auto player : PlayerPriorityMap)
	{
		// If player has higher priority save player and priority
		if (player.Value > highestPriority)
		{
			highestPriority = player.Value;
			highestPriorityPlayer = player.Key;
		}
	}

	
	if (highestPriority > 30)
	{
		CanPredictEngagement = false;
		return highestPriorityPlayer;
	}
	else
	{
		CanPredictEngagement = true;
	}

	// Predict an engagement if camera is in birds eye view
	if (CanPredictEngagement)
	{
		CanPredictEngagement = false;

		// Find a player about to engage in a fight
		if (PlayerEngagementActive)
		{
			return PlayerEngagementPrediction();
		}
	}

	// Birds eye camera view if no valid target
	return nullptr;
}

// Spawn an actor to track lifetime of a priority
void UAutoSpectatorComponent::SpawnPriorityTracker(int Priority, AController* Player, float Lifetime)
{
	World = GetWorld();
	
	if (World)
	{
		// Setup spawn actor variables
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		const FVector SpawnLocation = FVector::ZeroVector;

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn Priority tracker
		ASpectatePriorityTracker* PriorityTracker = World->SpawnActor<ASpectatePriorityTracker>(SpawnLocation, SpawnRotation, SpawnParameters);

		if (PriorityTracker)
		{
			PriorityTracker->SetDefaultParameters(Priority, Player, Lifetime, this);
		}
	}
}

// Increase or decrease the priority value of a player pawn
void UAutoSpectatorComponent::ChangePlayerSpectatePriority(int Priority, AController* Player, float Lifetime)
{
	// If player is null exit
	if (Player == nullptr)
	{
		return;
	}

	if (!PlayerPriorityMap.Contains(Player))
	{
		PlayerPriorityMap.Add(Player, Priority);
	}
	else
	{
		PlayerPriorityMap[Player] = PlayerPriorityMap[Player] + Priority;
	}

	// Spawn a tracker if priority is not permanent or expiring
	if (Lifetime != 0)
	{
		SpawnPriorityTracker(Priority, Player, Lifetime);
	}
}

// Select player pawn for spectate camera to spectate
AController* UAutoSpectatorComponent::SelectSpectateTarget()
{
	AController* SpectateTarget = FindHighestPriorityPlayer();

	// If nullptr spec target is birds eye view
	if (SpectateTarget != nullptr)
	{
		// Assure spectate target is different
		if (SpectateTarget == CurrentSpecTarget)
		{
			//ChangePlayerSpectatePriority(-500, SpectateTarget, 2);
			//SpectateTarget = FindHighestPriorityPlayer();
		}
		
		CurrentSpecTarget = SpectateTarget;
	}

	// Return new spectate target
	return SpectateTarget;
}

void UAutoSpectatorComponent::PlayerHeatMapPriority()
{
	// Find average position of players
	FVector AveragePosition = FVector::ZeroVector;
	
	for (auto player : PlayerPriorityMap)
	{
		if (player.Key->GetPawn())
		{
			AveragePosition += player.Key->GetPawn()->GetActorLocation();
		}
	}

	AveragePosition = AveragePosition / PlayerPriorityMap.Num();

	// Change priority based on distance from average
	for (auto player : PlayerPriorityMap)
	{
		if (player.Key->GetPawn())
		{
			// Find distance to average
			float distance = FVector::Distance(AveragePosition, player.Key->GetPawn()->GetActorLocation());
			
			// Normalise distance
			float maxDistance = 3000;
			float minDistance = 500;
			float normalisedDistance = (distance - minDistance) / (maxDistance - minDistance);

			// Clamp normalised between 0 and 1
			if (normalisedDistance < 0) normalisedDistance = 0;
			if (normalisedDistance > 1) normalisedDistance = 1;

			// Priority calculation
			float minPriority = 0;
			float maxPriority = 30;
			int priority = normalisedDistance * minPriority + (1 - normalisedDistance) * maxPriority;

			// Add priority to map
			PlayerPriorityMap[player.Key] = player.Value + priority;
			SpawnPriorityTracker(priority, player.Key, 1 / heatMapUpdateRate);
		}
	}
}

// Prioritise players that are about to start an engagement
AController* UAutoSpectatorComponent::PlayerEngagementPrediction()
{
	TMap<AController*, float> PlayerClosingSpeedMap;
	
	// For each player calculate their fastest closing speed to an enemy
	for (auto player : PlayerPriorityMap)
	{
		for (auto enemyPlayer : PlayerPriorityMap)
		{
			// Continue if player is enemy
			if (IsDifferentTeam(player.Key, enemyPlayer.Key))
			{
				// Skip iteration if players not in map
				if (!PreviousPlayerPositionMap.Contains(enemyPlayer.Key) || !PreviousPlayerPositionMap.Contains(player.Key))
				{
					continue;
				}
				
				// Previous distance
				FVector prevPlayerPos = PreviousPlayerPositionMap[player.Key];
				FVector prevEnemyPos = PreviousPlayerPositionMap[enemyPlayer.Key];
				
				// Current distance
				FVector currPlayerPos = FVector::ZeroVector;
				FVector currEnemyPos = FVector::ZeroVector;
				if (player.Key->GetPawn())	
					currPlayerPos = player.Key->GetPawn()->GetActorLocation();
				if (enemyPlayer.Key->GetPawn())
					currEnemyPos = enemyPlayer.Key->GetPawn()->GetActorLocation();
				
				// Calculate distances
				float prevDistance = FVector::Distance(prevEnemyPos, prevPlayerPos);
				float currentDistance = FVector::Distance(currEnemyPos, currPlayerPos);
				
				// Calculate distance difference
				float closingSpeed = prevDistance - currentDistance;

				// Replace or add new closing speed entry
				float maxDistance = 3000;
				if (PlayerClosingSpeedMap.Contains(player.Key) && currentDistance < maxDistance)
				{
					// Only accept the fastest closing speed
					if (closingSpeed > PlayerClosingSpeedMap[player.Key])
					{
						PlayerClosingSpeedMap[player.Key] = closingSpeed;
					}
				}
				else if (currentDistance < maxDistance)
				{
					PlayerClosingSpeedMap.Add(player.Key, closingSpeed);
				}
			}
		}

		// Update previous position
		if (player.Key->GetPawn())
			PreviousPlayerPositionMap.Add(player.Key, player.Key->GetPawn()->GetActorLocation());
	}
		
	// Return player with the highest closing speed above a minimum
	AController* TargetPlayer = nullptr;
	float minClosingSpeed = 800;
	for (auto player : PlayerClosingSpeedMap)
	{
		if (player.Value < minClosingSpeed)
		{
			continue;
		}
		
		if (TargetPlayer == nullptr)
		{
			TargetPlayer = player.Key;
		}
		
		if (PlayerClosingSpeedMap[TargetPlayer] < player.Value)
		{
			TargetPlayer = player.Key;
		}
	}
	
	return TargetPlayer;
}

bool UAutoSpectatorComponent::IsDifferentTeam(AController* PlayerOne, AController* PlayerTwo)
{
	AActor* Owner = GetOwner();
	
	if (Owner)
	{
		FName FunctionName = TEXT("IsOnDifferentTeam");
		UFunction* Function = Owner->FindFunction(FunctionName);

		if (Function)
		{
			struct FuncParams
			{
				AController* PlayerControllerOne;
				AController* PlayerControllerTwo;
				bool ReturnValue;
			};

			FuncParams Params;
			Params.PlayerControllerOne = PlayerOne;
			Params.PlayerControllerTwo = PlayerTwo;

			Owner->ProcessEvent(Function, &Params);

			return Params.ReturnValue;
		}
	}

	return false;
}

