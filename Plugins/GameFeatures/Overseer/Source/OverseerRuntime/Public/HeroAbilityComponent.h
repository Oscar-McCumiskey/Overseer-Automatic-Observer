#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "HeroAbilityComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, Category = "Hero Abilities", meta = (BlueprintSpawnableComponent))
class OVERSEERRUNTIME_API UHeroAbilityComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Hero Abilities")
	TSubclassOf<UGameplayAbility> GetGrantedAbility(int AbilityNum) const;

	UFUNCTION(BlueprintCallable, Category = "Hero Abilities")
	void SetAbility(TArray<TSubclassOf<UGameplayAbility>> AbilityClass);
	
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;
};
