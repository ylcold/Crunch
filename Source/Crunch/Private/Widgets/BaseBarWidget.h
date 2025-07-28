// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h" 
#include "BaseBarWidget.generated.h"

class UAbilitySystemComponent;
class UProgressBar;
class UTextBlock;

/**
 *
 */
UCLASS()
class UBaseBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	void SetAndBoundToGameplayAttributeBase(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayAttribute& Attribute, const FGameplayAttribute& MaxAttribute);

	void SetGaugeValueBase(float NewValue, float NewMaxValue);

private:
	UPROPERTY(EditAnywhere, Category = "Visual")
	FLinearColor BarBaseColor;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UProgressBar* BaseProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* ValueTextBase;

	float CachedValue;

	float CachedMaxValue;

	void UpdateGaugeAnimationBase();

	void ValueChangedBase(const FOnAttributeChangeData& Data);

	void MaxValueChangedBase(const FOnAttributeChangeData& Data);
};
