// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AutoSpectatorComponent.generated.h"

#include <map>


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
	void SpectateHighestPriorityPlayer();
	// Find player pawn with the highest priority in priority map
	APawn* FindHighestPriorityPlayer();
	// Increase or decrease the priority value of a player pawn
	void ChangePlayerSpectatePriority(int priority, APawn* player);
	// Attach spectate camera to a player pawn
	void AttachToSpectateTarget(APawn* SpectateTarget);
	// Select player pawn for spectate camera to spectate
	void SelectSpectateTarget(APawn* spectateTarget);
	// Remove the priority value of a player pawn from the priority map 
	void RemovePlayerFromMap(APawn* player);
	
	std::map<APawn*, int> playerPriorityMap;
	//std::list<> priorityList;
	
	bool hasChangedSpectateTarget = true;
	
	UPROPERTY()
	APawn* currentSpecTarget;
};
