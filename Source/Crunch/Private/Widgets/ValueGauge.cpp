// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/ValueGauge.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"
#include "Math/Color.h"

// 优化点：
// 1. 避免不必要的拷贝，尽量使用const引用和局部变量。
// 2. 减少重复计算。
// 3. 用getter/setter替换对WidgetStyle的直接访问，避免弃用警告。
// 4. 静态格式化选项只初始化一次。
// 5. 代码结构更清晰，易于维护。

namespace
{
	static const FNumberFormattingOptions& GetNumberFormatOptions()
	{
		static const FNumberFormattingOptions Options = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
		return Options;
	}
}

void UValueGauge::NativePreConstruct()
{
	Super::NativePreConstruct();

	// 设置主血条背景色（如深灰色）
	FProgressBarStyle Style = ProgressBar->GetWidgetStyle();
	Style.BackgroundImage.TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 1.f));
	ProgressBar->SetWidgetStyle(Style);

	DelayedBar->SetFillColorAndOpacity(FLinearColor(0.8f, 0.f, 0.f, 0.4f));
}

void UValueGauge::SetAndBoundToGameplayAttribute(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayAttribute& Attribute, const FGameplayAttribute& MaxAttribute)
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
		SetGaugeValue(Value, MaxValue);
	}
	else
	{
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

void UValueGauge::SetGaugeValue(float NewValue, float NewMaxValue)
{
	CachedValue = NewValue;
	CachedMaxValue = NewMaxValue;

	if (NewMaxValue <= 0.0f)
	{
		ProgressBar->SetPercent(0.0f);
		DelayedBar->SetPercent(0.0f);
		ValueText->SetText(FText::FromString(TEXT("0 / 0")));
		return;
	}

	const float NewPercent = FMath::Clamp(NewValue / NewMaxValue, 0.0f, 1.0f);
	ProgressBar->SetPercent(NewPercent);

	// 如果掉血，启动延迟条动画
	if (NewPercent < DelayedPercent)
	{
		GetWorld()->GetTimerManager().SetTimer(
			DelayBarTimerHandle,
			this,
			&UValueGauge::UpdateDelayedBar,
			0.01f,
			true
		);
	}
	else
	{
		DelayedPercent = NewPercent;
		DelayedBar->SetPercent(DelayedPercent);
	}

	// 更新文本显示
	ValueText->SetText(
		FText::Format(
			FTextFormat::FromString(TEXT("{0} / {1}")),
			FText::AsNumber(NewValue, &GetNumberFormatOptions()),
			FText::AsNumber(NewMaxValue, &GetNumberFormatOptions())
		)
	);
}

void UValueGauge::UpdateDelayedBar()
{
	// 以一定速度追赶ImmediateBar
	const float Speed = 3.f; // 每秒减少的百分比，可调
	const float Target = ProgressBar->GetPercent();
	DelayedPercent = FMath::FInterpTo(DelayedPercent, Target, 0.01f, Speed);
	DelayedBar->SetPercent(DelayedPercent);

	if (FMath::IsNearlyEqual(DelayedPercent, Target, 0.001f))
	{
		DelayedPercent = Target;
		DelayedBar->SetPercent(DelayedPercent);
		GetWorld()->GetTimerManager().ClearTimer(DelayBarTimerHandle);
	}
}

void UValueGauge::SetGaugeValueOther(float NewValue, float NewMaxValue)
{
	CachedValue = NewValue;
	CachedMaxValue = NewMaxValue;

	if (NewMaxValue <= 0.0f)
	{
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
		0.01f,
		true
	);

	const float NewPercent = TargetPercent;
	ProgressBar->SetPercent(NewPercent);

	// 如果掉血，启动延迟条动画
	if (NewPercent < DelayedPercent)
	{
		GetWorld()->GetTimerManager().SetTimer(
			DelayBarTimerHandle,
			this,
			&UValueGauge::UpdateDelayedBar,
			0.01f,
			true
		);
	}
	else
	{
		DelayedPercent = NewPercent;
		DelayedBar->SetPercent(DelayedPercent);
	}

	ValueText->SetText(
		FText::Format(
			FTextFormat::FromString(TEXT("{0} / {1}")),
			FText::AsNumber(NewValue, &GetNumberFormatOptions()),
			FText::AsNumber(NewMaxValue, &GetNumberFormatOptions())
		)
	);
}

void UValueGauge::UpdateGaugeAnimation()
{
	AnimElapsed += 0.01f;
	const float Alpha = FMath::Clamp(AnimElapsed / AnimDuration, 0.0f, 1.0f);
	const float NewPercent = FMath::Lerp(CurrentPercent, TargetPercent, Alpha);
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
