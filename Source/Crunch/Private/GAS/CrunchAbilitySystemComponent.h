// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CrunchAbilitySystemComponent.generated.h"


class UGameplayEffect;

/**
 * 
 */
UCLASS()
class UCrunchAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

public:
	void ApplyInitialEffects();
};
