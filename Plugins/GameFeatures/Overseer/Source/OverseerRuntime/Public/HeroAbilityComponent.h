// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "HeroAbilityComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Category = "Hero Abilities", meta = (BlueprintSpawnableComponents))
class OVERSEERRUNTIME_API UHeroAbilityComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Hero Abilities")
	void SetPrimaryAbility(FGameplayAbilitySpecHandle primaryAbility);

	UFUNCTION(BlueprintCallable, Category = "Hero Abilities")
	void SetSecondaryAbility(FGameplayAbilitySpecHandle secondaryAbility);

	UFUNCTION(BlueprintCallable, Category = "Hero Abilities")
	void ClearPrimaryAbility();

	UFUNCTION(BlueprintCallable, Category = "Hero Abilities")
	void ClearSecondaryAbility();
};
