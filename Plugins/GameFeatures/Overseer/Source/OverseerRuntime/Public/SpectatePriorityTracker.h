// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpectatePriorityTracker.generated.h"

class UAutoSpectatorComponent;

UCLASS()
class OVERSEERRUNTIME_API ASpectatePriorityTracker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpectatePriorityTracker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDefaultParameters(int Priority, AController* Controller, float Lifetime, UAutoSpectatorComponent* Asc);

private:
	int Priority;
	float Lifetime;

	UPROPERTY()
	AController* Controller;

	UPROPERTY()
	UAutoSpectatorComponent* Asc;
};
