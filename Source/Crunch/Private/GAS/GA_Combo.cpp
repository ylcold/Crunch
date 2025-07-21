// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Combo.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "CrunchGameplayTags.h"


UGA_Combo::UGA_Combo()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(CrunchGameplayTags::Ability_BasicAttack);
	SetAssetTags(Tags);

	BlockAbilitiesWithTag.AddTag(CrunchGameplayTags::Ability_BasicAttack);
}

void UGA_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	//{
	//	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	//	return;
	//}

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}


	/*UE_LOG(LogTemp, Log, TEXT("Combo ability activated for %s"), *ActorInfo->OwnerActor->GetName());*/


	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			ComboMontage
		);

		MontageTask->OnCompleted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		MontageTask->ReadyForActivation();
	}
}
