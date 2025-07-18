// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CrunchCharacter.generated.h"

class UCrunchAbilitySystemComponent;
class UCrunchAttributeSet;
class UWidgetComponent;

/**
 * 角色基类，集成能力系统
 */
UCLASS()
class ACrunchCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// 构造函数
	ACrunchCharacter();

	// IAbilitySystemInterface 实现
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 初始化
	void ServerSideInit();
	void ClientSideInit();

	bool IsLocallyControlledByPlayer() const;

	virtual void PossessedBy(AController* NewController) override;

protected:
	// 生命周期
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// 能力系统组件
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability")
	TObjectPtr<UCrunchAbilitySystemComponent> CrunchAbilitySystemComponent;

	// 属性集
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability")
	TObjectPtr<UCrunchAttributeSet> CrunchAttributeSet;

	// 头顶UI组件
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability")
	TObjectPtr<UWidgetComponent> OverHeadWidgetComponent;

	void ConfigureOverHeadWidgetComponent();
};
