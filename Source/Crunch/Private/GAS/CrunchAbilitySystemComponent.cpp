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

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : Abilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 0, INDEX_NONE, nullptr);

			GiveAbility(AbilitySpec);
		}
	}

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : BasicAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, nullptr);
			GiveAbility(AbilitySpec);
		}
	}
}
