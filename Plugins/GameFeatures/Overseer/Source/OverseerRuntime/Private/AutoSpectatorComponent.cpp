// Fill out your copyright notice in the Description page of Project Settings.

#include "AutoSpectatorComponent.h"

// Sets default values for this component's properties
UAutoSpectatorComponent::UAutoSpectatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ..

	currentSpecTarget = nullptr;
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

// Find player pawn with the highest priority in priority map
AController* UAutoSpectatorComponent::FindHighestPriorityPlayer()
{
	AController* highestPriorityPlayer = nullptr;
	int highestPriority = -1;
	
	for (auto player : playerPriorityMap)
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

// Increase or decrease the priority value of a player pawn
void UAutoSpectatorComponent::ChangePlayerSpectatePriority(int priority, AController* player, float lifetime)
{
	// If player is null exit
	if (player == nullptr)
	{
		return;
	}

	if (!playerPriorityMap.Contains(player))
	{
		playerPriorityMap.Add(player, priority);
	}
	else
	{
		playerPriorityMap[player] = playerPriorityMap[player] + priority;
	}
	
}

// Select player pawn for spectate camera to spectate
AController* UAutoSpectatorComponent::SelectSpectateTarget()
{
	AController* spectateTarget = FindHighestPriorityPlayer();
	
	// Assure spectate target is different
	if (spectateTarget == currentSpecTarget && spectateTarget != nullptr)
	{
		ChangePlayerSpectatePriority(-500, spectateTarget);
		spectateTarget = FindHighestPriorityPlayer();
	}

	if (spectateTarget != nullptr)
	{
		currentSpecTarget = spectateTarget;
	}
	
	// Return new spectate target
	return spectateTarget;
}

