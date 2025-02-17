// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>

#include <map>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AutoSpectatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERSEERRUNTIME_API UAutoSpectatorComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UAutoSpectatorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Increase or decrease the priority value of a player pawn
	void ChangePlayerSpectatePriority(int priority, APawn* player, float lifetime = 0);
	// Select player pawn for observer camera to spectate
	APawn* SelectSpectateTarget();
	// Find player pawn with the highest priority in priority map
	APawn* FindHighestPriorityPlayer();
	// Remove the priority value of a player pawn from the priority map 
	void RemovePlayerFromMap(APawn* player);
	
	std::map<APawn*, int> playerPriorityMap;
	//std::list<> priorityList;
	
	bool hasChangedSpectateTarget = true;
	
	UPROPERTY()
	APawn* currentSpecTarget;
};
