// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/CrunchAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCrunchAnimInstance::NativeInitializeAnimation()
{
	OwningPlayerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwningPlayerCharacter)
	{
		OwningCharacterMovementComponent = OwningPlayerCharacter->GetCharacterMovement();
	}
	else
	{
		OwningCharacterMovementComponent = nullptr;
	}
}

void UCrunchAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (OwningPlayerCharacter)
	{
		Speed = OwningPlayerCharacter->GetVelocity().Length();
		FRotator CurrentRotation = OwningPlayerCharacter->GetActorRotation();
		FRotator RotationDelta = UKismetMathLibrary::NormalizedDeltaRotator(CurrentRotation, PrevRotation);
		PrevRotation = CurrentRotation;

		YawSpeed = RotationDelta.Yaw / DeltaSeconds;
		SmoothYawSpeed = UKismetMathLibrary::FInterpTo(SmoothYawSpeed, YawSpeed, DeltaSeconds, YawSpeedSmoothing);

		FRotator ControlledRotation = OwningPlayerCharacter->GetBaseAimRotation();
		LookRotationOffset = UKismetMathLibrary::NormalizedDeltaRotator(ControlledRotation, CurrentRotation);
	}

	if (OwningCharacterMovementComponent)
	{
		bIsJumping = OwningCharacterMovementComponent->IsFalling();
	}
}

void UCrunchAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
}
