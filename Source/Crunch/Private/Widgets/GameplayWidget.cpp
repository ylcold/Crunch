// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameplayWidget.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Widgets/ValueGauge.h"
#include "GAS/CrunchAttributeSet.h"


void UGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());

	if (AbilitySystemComponent)
	{
		HealthBar->SetAndBoundToGameplayAttribute(AbilitySystemComponent, UCrunchAttributeSet::GetHealthAttribute(), UCrunchAttributeSet::GetMaxHealthAttribute());
		ManaBar->SetAndBoundToGameplayAttribute(AbilitySystemComponent, UCrunchAttributeSet::GetManaAttribute(), UCrunchAttributeSet::GetMaxManaAttribute());
	}
}
