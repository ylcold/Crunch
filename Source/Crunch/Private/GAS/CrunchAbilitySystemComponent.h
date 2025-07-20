// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CrunchAbilitySystemComponent.generated.h"


class UGameplayEffect;
class UGameplayAbility;

/**
 * 
 */
UCLASS()
class UCrunchAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> BasicAbilities;

public:
	void ApplyInitialEffects();

	void GiveAbilities();
};
