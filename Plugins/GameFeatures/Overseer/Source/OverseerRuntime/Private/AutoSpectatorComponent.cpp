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
void UAutoSpectatorComponent::ChangePlayerSpectatePriority(int priority, APawn* player, float lifetime)
{
	// Priority change is permanent
	if (lifetime <= 0)
	{
		playerPriorityMap.at(player) = playerPriorityMap.at(player) + priority;
	}
	// Priority change is temporary
	else
	{
		playerPriorityMap.at(player) = playerPriorityMap.at(player) + priority;
	}
	
}

// Select player pawn for spectate camera to spectate
APawn* UAutoSpectatorComponent::SelectSpectateTarget()
{
	APawn* spectateTarget = FindHighestPriorityPlayer();
	
	// Assure spectate target is different
	if (spectateTarget == currentSpecTarget)
	{
		ChangePlayerSpectatePriority(-500, spectateTarget);
		spectateTarget = FindHighestPriorityPlayer();
	}

	currentSpecTarget = spectateTarget;
	// Return new spectate target
	return spectateTarget;
}

// Remove the priority value of a player pawn from the priority map
void UAutoSpectatorComponent::RemovePlayerFromMap(APawn* player)
{
	ChangePlayerSpectatePriority(-999, player);
	//playerPriorityMap.erase(player);
}

