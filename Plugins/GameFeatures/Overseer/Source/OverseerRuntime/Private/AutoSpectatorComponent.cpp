// Fill out your copyright notice in the Description page of Project Settings.

#include "AutoSpectatorComponent.h"

// Sets default values for this component's properties
UAutoSpectatorComponent::UAutoSpectatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ..
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
}

void UAutoSpectatorComponent::SpectateHighestPriorityPlayer()
{
	APawn* HighestPriorityPawn = FindHighestPriorityPlayer();
	
	if (HighestPriorityPawn == currentSpecTarget)
	{
		// Spectate target not changed
		hasChangedSpectateTarget = false;
	}
	else
	{
		// Change spectate target
		SelectSpectateTarget(HighestPriorityPawn);
		currentSpecTarget = HighestPriorityPawn;
		hasChangedSpectateTarget = true;
	}
}

// Find player pawn with the highest priority in priority map
APawn* UAutoSpectatorComponent::FindHighestPriorityPlayer()
{
	APawn* highestPriorityPlayer = nullptr;
	int highestPriority = -1;
	
	for (auto player : playerPriorityMap)
	{
		// If player has higher priority save player and priority
		if (player.second > highestPriority)
		{
			highestPriority = player.second;
			highestPriorityPlayer = player.first;
		}
	}
	return highestPriorityPlayer;
}

// Increase or decrease the priority value of a player pawn
void UAutoSpectatorComponent::ChangePlayerSpectatePriority(int priority, APawn* player)
{
	playerPriorityMap.at(player) = playerPriorityMap.at(player) + priority;
}

// Attach spectate camera to a player pawn
void UAutoSpectatorComponent::AttachToSpectateTarget(APawn* SpectateTarget)
{
}

// Select player pawn for spectate camera to spectate
void UAutoSpectatorComponent::SelectSpectateTarget(APawn* spectateTarget)
{
}

// Remove the priority value of a player pawn from the priority map
void UAutoSpectatorComponent::RemovePlayerFromMap(APawn* player)
{
}

