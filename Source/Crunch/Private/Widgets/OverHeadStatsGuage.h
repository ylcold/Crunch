// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverHeadStatsGuage.generated.h"

class UAbilitySystemComponent;
class UValueGauge;
class UBaseBarWidget;
/**
 *
 */
UCLASS()
class UOverHeadStatsGuage : public UUserWidget
{
	GENERATED_BODY()

public:
	void ConfigureWithASC(UAbilitySystemComponent* AbilitySystemComponent);

private:
	UPROPERTY(meta = (BindWidget))
	UValueGauge* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UBaseBarWidget* ManaBar;
};