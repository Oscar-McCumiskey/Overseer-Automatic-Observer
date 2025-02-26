// Fill out your copyright notice in the Description page of Project Settings.

#include "AutoSpectatorComponent.h"

#include "SpectatePriorityTracker.h"

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

	// Heat map timer
	if (heatMapTimer > 0)
	{
		heatMapTimer-= DeltaTime;
	}
	else
	{
		// Update heat map
		heatMapTimer = 1 / heatMapUpdateRate;
		//PlayerHeatMapPriority();
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
	return highestPriorityPlayer;
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
	
	// Assure spectate target is different
	if (SpectateTarget == CurrentSpecTarget && SpectateTarget != nullptr)
	{
		ChangePlayerSpectatePriority(-500, SpectateTarget, 2);
		SpectateTarget = FindHighestPriorityPlayer();
	}

	if (SpectateTarget != nullptr)
	{
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
		AveragePosition += player.Key->GetPawn()->GetActorLocation();
	}

	AveragePosition = AveragePosition / PlayerPriorityMap.Num();

	// Change priority based on distance from average
	for (auto player : PlayerPriorityMap)
	{
		// Find distance to average
		float distance = FVector::Distance(AveragePosition, player.Key->GetPawn()->GetActorLocation());

		// Adjust priority in map
		float multiplier = 100;
		int priority = multiplier / distance;
		
		PlayerPriorityMap[player.Key] = player.Value + priority;
		SpawnPriorityTracker(priority, player.Key, 1 / heatMapUpdateRate);
	}
}

