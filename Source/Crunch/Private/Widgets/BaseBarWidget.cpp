// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/BaseBarWidget.h"
#include "Components/ProgressBar.h"
#include "AbilitySystemComponent.h"
#include "Components/TextBlock.h"


void UBaseBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	// Additional pre-construct logic can be added here if needed

	BaseProgressBar->SetFillColorAndOpacity(BarBaseColor);
}

void UBaseBarWidget::SetAndBoundToGameplayAttributeBase(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayAttribute& Attribute, const FGameplayAttribute& MaxAttribute)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	bool bFound = false;
	const float Value = AbilitySystemComponent->GetGameplayAttributeValue(Attribute, bFound);
	const float MaxValue = AbilitySystemComponent->GetGameplayAttributeValue(MaxAttribute, bFound);

	if (bFound)
	{
		SetGaugeValueBase(Value, MaxValue);
        
	}
	else
	{
		BaseProgressBar->SetPercent(0.0f);
		ValueTextBase->SetText(FText::FromString(TEXT("0 / 0")));
	}

	// 先解绑，防止重复绑定
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute).RemoveAll(this);

	// 绑定属性变化委托
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UBaseBarWidget::ValueChangedBase);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddUObject(this, &UBaseBarWidget::MaxValueChangedBase);
}

void UBaseBarWidget::SetGaugeValueBase(float NewValue, float NewMaxValue)
{
	CachedValue = NewValue;
	CachedMaxValue = NewMaxValue;

	if (CachedMaxValue > 0)
	{
		BaseProgressBar->SetPercent(CachedValue / CachedMaxValue);
		ValueTextBase->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CachedValue, CachedMaxValue)));
	}
	else
	{
		BaseProgressBar->SetPercent(0.0f);
		ValueTextBase->SetText(FText::FromString(TEXT("0 / 0")));
	}
}

void UBaseBarWidget::UpdateGaugeAnimationBase()
{
}

void UBaseBarWidget::ValueChangedBase(const FOnAttributeChangeData& Data)
{
	SetGaugeValueBase(Data.NewValue, CachedMaxValue);
}

void UBaseBarWidget::MaxValueChangedBase(const FOnAttributeChangeData& Data)
{
	SetGaugeValueBase(CachedValue, Data.NewValue);
}