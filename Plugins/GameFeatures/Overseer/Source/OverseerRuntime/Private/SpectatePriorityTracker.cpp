// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectatePriorityTracker.h"

#include "AutoSpectatorComponent.h"

// Sets default values
ASpectatePriorityTracker::ASpectatePriorityTracker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Lifetime = 0;
	Priority = 0;
	Controller = nullptr;
	Asc = nullptr;
}

// Called when the game starts or when spawned
void ASpectatePriorityTracker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpectatePriorityTracker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If lifetime over remove priority from map and destroy this actor
	if (Lifetime <= 0)
	{
		Asc->ChangePlayerSpectatePriority(Priority, Controller);

		if (Priority == 999 || Priority == -999)
		{
			//Asc->PlayerPriorityMap.Remove(Controller);
		}
		this->Destroy();
	}
	
	Lifetime -= DeltaTime;
}

void ASpectatePriorityTracker::SetDefaultParameters(int IPriority, AController* IController, float ILifetime, UAutoSpectatorComponent* IAsc)
{
	this->Priority = -IPriority;
	this->Lifetime = ILifetime;
	this->Controller = IController;
	this->Asc = IAsc;
}

