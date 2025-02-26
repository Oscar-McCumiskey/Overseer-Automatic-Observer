// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	void ChangePlayerSpectatePriority(int Priority, AController* Player, float Lifetime = 0);

	UPROPERTY(BlueprintReadWrite)
	AController* CurrentSpecTarget;

	UPROPERTY(BlueprintReadWrite)
	TMap<AController*, int> PlayerPriorityMap;
	
private:
	// Find player pawn with the highest priority in priority map
	AController* FindHighestPriorityPlayer();

	// Spawn an actor to track lifetime of a priority
	void SpawnPriorityTracker(int Priority, AController* Player, float Lifetime);

	bool bHasChangedSpectateTarget = true;

	// Pointer to world
	UPROPERTY()
	UWorld* World;
};
