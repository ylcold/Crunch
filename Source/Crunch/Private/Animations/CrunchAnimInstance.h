// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CrunchAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;
/**
 *
 */
UCLASS()
class UCrunchAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// the below functions are the native overrides for each phase
	// Native initialization override point
	virtual void NativeInitializeAnimation() override;

	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeThreadSafeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Native thread safe update override point. Executed on a worker thread just prior to graph update 
	// for linked anim instances, only called when the hosting node(s) are relevant
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Crunch|Animation", meta = (BlueprintThreadSafe))
	FORCEINLINE float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintCallable, Category = "Crunch|Animation", meta = (BlueprintThreadSafe))
	FORCEINLINE bool IsMoving() const { return Speed > 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "Crunch|Animation", meta = (BlueprintThreadSafe))
	FORCEINLINE bool IsNotMoving() const { return Speed <= 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "Crunch|Animation", meta = (BlueprintThreadSafe))
	FORCEINLINE float GetYawSpeed() const { return YawSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Crunch|Animation", meta = (BlueprintThreadSafe))
	FORCEINLINE float GetSmoothYawSpeed() const { return SmoothYawSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Crunch|Animation", meta = (BlueprintThreadSafe))
	FORCEINLINE bool GetIsJumping() const { return bIsJumping; }

	UFUNCTION(BlueprintCallable, Category = "Crunch|Animation", meta = (BlueprintThreadSafe))
	FORCEINLINE bool GetIsOnGround() const { return !bIsJumping; }

private:
	UPROPERTY()
	TObjectPtr<ACharacter> OwningPlayerCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwningCharacterMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crunch|Animation", meta = (AllowPrivateAccess = "true"))
	float YawSpeedSmoothing = 1.f;

	float Speed;
	float YawSpeed;
	float SmoothYawSpeed;
	bool bIsJumping;

	FRotator PrevRotation;
};
