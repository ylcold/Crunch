// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayWidget.generated.h"


class UValueGauge;
class UBaseBarWidget;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UValueGauge* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UBaseBarWidget* ManaBar;

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
};
