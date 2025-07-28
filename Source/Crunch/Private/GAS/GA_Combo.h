// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CrunchGameplayAbility.h"
#include "GA_Combo.generated.h"

/**
 *
 */
UCLASS()
class UGA_Combo : public UCrunchGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Combo();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	static FGameplayTag GetComboChangeEventTag();
	static FGameplayTag GetComboTargetEventTag();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ComboMontage;

	UFUNCTION()
	void OnComboEventReceived(FGameplayEventData Payload);

	FName NextComboSectionName;

	void SetupWaitComboInputPress();

	UFUNCTION()
	void OnComboInputPressed(float TimeWaited);

	void TryCommitCombo();

	UFUNCTION()
	void DoDamage(FGameplayEventData Payload);

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TMap<FName, TSubclassOf<UGameplayEffect>> DamageEffectMap;

	TSubclassOf<UGameplayEffect> GetDamageEffectForCurrentCombo() const;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TSubclassOf<UGameplayEffect> DefaultDamageEffect;

};