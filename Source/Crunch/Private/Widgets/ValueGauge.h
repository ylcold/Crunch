// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h" // For FOnAttributeChangeData
#include "ValueGauge.generated.h"


class UAbilitySystemComponent;
class UProgressBar;
class UTextBlock;
/**
 *
 */
UCLASS()
class UValueGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	void SetAndBoundToGameplayAttribute(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayAttribute& Attribute, const FGameplayAttribute & MaxAttribute);
	void SetGaugeValue(float NewValue, float NewMaxValue);

private:
	UPROPERTY(EditAnywhere, Category = "Visual")
	FLinearColor BarColor;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* ValueText;

	FTimerHandle GaugeAnimTimerHandle;
	float CurrentPercent = 0.0f;
	float TargetPercent = 0.0f;
	float AnimDuration = 0.3f; // ∂Øª≠ ±≥§£®√Î£©
	float AnimElapsed = 0.0f;

	UFUNCTION()
	void UpdateGaugeAnimation();

	float CachedValue;
	float CachedMaxValue;

	void ValueChanged(const FOnAttributeChangeData& Data);

	void MaxValueChanged(const FOnAttributeChangeData& Data);
};
