// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ValueGauge.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"

void UValueGauge::NativePreConstruct()
{
	Super::NativePreConstruct();

	ProgressBar->SetFillColorAndOpacity(BarColor);
}

void UValueGauge::SetAndBoundToGameplayAttribute(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayAttribute& Attribute, const FGameplayAttribute& MaxAttribute)
{
	if (AbilitySystemComponent)
	{
		bool bFound;
		float Value = AbilitySystemComponent->GetGameplayAttributeValue(Attribute, bFound);
		float MaxValue = AbilitySystemComponent->GetGameplayAttributeValue(MaxAttribute, bFound);

		if (bFound)
		{
			SetGaugeValue(Value, MaxValue);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find attribute value for %s or %s"), *Attribute.GetName(), *MaxAttribute.GetName());
			ProgressBar->SetPercent(0.0f);
			ValueText->SetText(FText::FromString(TEXT("0 / 0")));
		}

		// 先解绑，防止重复绑定
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute).RemoveAll(this);

		// 绑定属性变化委托
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UValueGauge::ValueChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddUObject(this, &UValueGauge::MaxValueChanged);
	}
}

void UValueGauge::SetGaugeValue(float NewValue, float NewMaxValue)
{

	CachedValue = NewValue;
	CachedMaxValue = NewMaxValue;

	if (NewMaxValue <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("NewMaxValue is zero or negative, cannot set gauge value."));
		ProgressBar->SetPercent(0.0f);
		ValueText->SetText(FText::FromString(TEXT("0 / 0")));
		return;
	}

	TargetPercent = FMath::Clamp(NewValue / NewMaxValue, 0.0f, 1.0f);
	AnimElapsed = 0.0f;

	// 停止之前的动画
	GetWorld()->GetTimerManager().ClearTimer(GaugeAnimTimerHandle);

	// 启动动画定时器
	GetWorld()->GetTimerManager().SetTimer(
		GaugeAnimTimerHandle,
		this,
		&UValueGauge::UpdateGaugeAnimation,
		0.01f, // 每帧更新时间
		true
	);

	static const FNumberFormattingOptions Options = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	ValueText->SetText(
		FText::Format(
			FTextFormat::FromString(TEXT("{0} / {1}")),
			FText::AsNumber(NewValue, &Options),
			FText::AsNumber(NewMaxValue, &Options)
		)
	);
}

void UValueGauge::UpdateGaugeAnimation()
{
	AnimElapsed += 0.01f;
	float Alpha = FMath::Clamp(AnimElapsed / AnimDuration, 0.0f, 1.0f);
	float NewPercent = FMath::Lerp(CurrentPercent, TargetPercent, Alpha);
	ProgressBar->SetPercent(NewPercent);

	if (Alpha >= 1.0f)
	{
		CurrentPercent = TargetPercent;
		GetWorld()->GetTimerManager().ClearTimer(GaugeAnimTimerHandle);
	}
}

void UValueGauge::ValueChanged(const FOnAttributeChangeData& Data)
{
	SetGaugeValue(Data.NewValue, CachedMaxValue);
}

void UValueGauge::MaxValueChanged(const FOnAttributeChangeData& Data)
{
	SetGaugeValue(CachedValue, Data.NewValue);
}
