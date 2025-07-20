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
	/*UGA_Combo(const FObjectInitializer& ObjectInitializer);*/
	// Override the ActivateAbility method to implement combo logic
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
