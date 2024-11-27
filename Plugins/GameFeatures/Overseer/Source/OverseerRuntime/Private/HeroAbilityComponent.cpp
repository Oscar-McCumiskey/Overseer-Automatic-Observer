#include "HeroAbilityComponent.h"

TSubclassOf<UGameplayAbility> UHeroAbilityComponent::GetGrantedAbility(int AbilityNum) const
{
	return GrantedAbilities[AbilityNum];
}

void UHeroAbilityComponent::SetAbility(TArray<TSubclassOf<UGameplayAbility>> AbilityClass)
{
	for (TSubclassOf<UGameplayAbility> Ability : AbilityClass)
	{
		GrantedAbilities.Add(Ability);
	} 
}
