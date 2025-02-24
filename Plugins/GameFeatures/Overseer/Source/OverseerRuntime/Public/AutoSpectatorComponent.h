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

	// Select player pawn for observer camera to spectate
	UFUNCTION(BlueprintCallable)
	AController* SelectSpectateTarget();

	// Increase or decrease the priority value of a player pawn
	UFUNCTION(BlueprintCallable)
	void ChangePlayerSpectatePriority(int priority, AController* player, float lifetime = 0);

	UPROPERTY(BlueprintReadWrite)
	AController* currentSpecTarget;

	UPROPERTY(BlueprintReadWrite)
	TMap<AController*, int> playerPriorityMap;
	
private:
	// Find player pawn with the highest priority in priority map
	AController* FindHighestPriorityPlayer();

	bool hasChangedSpectateTarget = true;
};
