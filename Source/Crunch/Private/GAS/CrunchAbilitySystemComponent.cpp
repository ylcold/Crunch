// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CrunchAbilitySystemComponent.h"


void UCrunchAbilitySystemComponent::ApplyInitialEffects()
{

	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	for (const TSubclassOf<UGameplayEffect>& EffectClass : InitialEffects)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(EffectClass, 1, MakeEffectContext());

		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void UCrunchAbilitySystemComponent::GiveAbilities()
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	for (const TPair<ECrunchAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityPair.Value, 0, (int32)AbilityPair.Key, nullptr);
		GiveAbility(AbilitySpec);
	}

	for (const TPair<ECrunchAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : BasicAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityPair.Value, 1, (int32)AbilityPair.Key, nullptr);
		GiveAbility(AbilitySpec);
	}
}
