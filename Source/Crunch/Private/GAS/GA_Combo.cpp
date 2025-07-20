// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Combo.h"

void UGA_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// Implement your ability activation logic here
	// For example, you might want to start a combo attack sequence or apply effects

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Combo ability activated for %s"), *ActorInfo->OwnerActor->GetName());
}
