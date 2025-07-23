// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CrunchGameplayAbility.generated.h"

class UAnimInstance;
/**
 *  游戏能力基类
 */
UCLASS()
class UCrunchGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UAnimInstance* GetOwnerAnimInstance() const;
};
