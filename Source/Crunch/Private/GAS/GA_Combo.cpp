// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Combo.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "CrunchGameplayTags.h"
#include "GameplayTagsManager.h"
#include "AbilitySystemBlueprintLibrary.h"


UGA_Combo::UGA_Combo()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(CrunchGameplayTags::Ability_BasicAttack);
	SetAssetTags(Tags);

	BlockAbilitiesWithTag.AddTag(CrunchGameplayTags::Ability_BasicAttack);
}

void UGA_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

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

		UAbilityTask_WaitGameplayEvent* WaitComboChangeEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			GetComboChangeEventTag(),
			nullptr,
			false,
			false
		);
		WaitComboChangeEventTask->EventReceived.AddDynamic(this, &UGA_Combo::OnComboEventReceived);
		WaitComboChangeEventTask->ReadyForActivation();
	}

	if (K2_HasAuthority())
	{
		UAbilityTask_WaitGameplayEvent* WaitTargetingEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			CrunchGameplayTags::Ability_Combo_Damage
		);

		WaitTargetingEventTask->EventReceived.AddDynamic(this, &UGA_Combo::DoDamage);
		WaitTargetingEventTask->ReadyForActivation();
	}

	SetupWaitComboInputPress();
}

FGameplayTag UGA_Combo::GetComboChangeEventTag()
{
	return FGameplayTag::RequestGameplayTag(FName(TEXT("Ability.Combo.Change")));
}

FGameplayTag UGA_Combo::GetComboTargetEventTag()
{
	return FGameplayTag::RequestGameplayTag(FName(TEXT("Ability.Combo.Damage")));
}

void UGA_Combo::OnComboEventReceived(FGameplayEventData Payload)
{
	FGameplayTag EventTag = Payload.EventTag;
	if (EventTag == CrunchGameplayTags::Ability_Combo_Change_End)
	{
		NextComboSectionName = NAME_None;
		return;
	}

	TArray<FName> ComboSections;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, ComboSections);
	NextComboSectionName = ComboSections.Last();

	/*UE_LOG(LogTemp, Log, TEXT("Combo section changed to: %s"), *NextComboSectionName.ToString());*/
}

void UGA_Combo::SetupWaitComboInputPress()
{
	UAbilityTask_WaitInputPress* WaitInputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPressTask->OnPress.AddDynamic(this, &UGA_Combo::OnComboInputPressed);
	WaitInputPressTask->ReadyForActivation();
}

void UGA_Combo::OnComboInputPressed(float TimeWaited)
{
	SetupWaitComboInputPress();

	TryCommitCombo();
}

void UGA_Combo::TryCommitCombo()
{

	if (NextComboSectionName == NAME_None)
	{
		return;
	}


	UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance();

	if (!OwnerAnimInstance)
	{
		return;
	}

	OwnerAnimInstance->Montage_SetNextSection(
		OwnerAnimInstance->Montage_GetCurrentSection(ComboMontage),
		NextComboSectionName,
		ComboMontage
	);
}

void UGA_Combo::DoDamage(FGameplayEventData Payload)
{
	TArray<FHitResult> HitResults = GetHitResultsSweepLocationTargetData(Payload.TargetData, 30.f, true, true);

	for (const FHitResult& Hit : HitResults)
	{
		if (Hit.GetActor() && Hit.GetActor() != GetAvatarActorFromActorInfo())
		{
			TSubclassOf<UGameplayEffect> DamageEffect = GetDamageEffectForCurrentCombo();
			FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffect, GetAbilityLevel(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo()));

			ApplyGameplayEffectSpecToTarget(
				GetCurrentAbilitySpecHandle(),
				CurrentActorInfo,
				CurrentActivationInfo,
				DamageSpecHandle,
				UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(Hit.GetActor())
			);
		}
	}
}

TSubclassOf<UGameplayEffect> UGA_Combo::GetDamageEffectForCurrentCombo() const
{
	UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance();
	if (OwnerAnimInstance)
	{
		FName CurrentSectionName = OwnerAnimInstance->Montage_GetCurrentSection(ComboMontage);
		const TSubclassOf<UGameplayEffect>* EffectPtr = DamageEffectMap.Find(CurrentSectionName);
		if (EffectPtr)
		{
			return *EffectPtr;
		}
	}
	return DefaultDamageEffect;
}
