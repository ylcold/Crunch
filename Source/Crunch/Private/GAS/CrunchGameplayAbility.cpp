// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CrunchGameplayAbility.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UAnimInstance* UCrunchGameplayAbility::GetOwnerAnimInstance() const
{
	USkeletalMeshComponent* OwnerSkeletalMeshComponent = GetOwningComponentFromActorInfo();
	if (OwnerSkeletalMeshComponent)
	{
		return OwnerSkeletalMeshComponent->GetAnimInstance();
	}
	return nullptr;
}

TArray<FHitResult> UCrunchGameplayAbility::GetHitResultsSweepLocationTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, float SphereSweepRadius, bool bDrawDebug, bool bIgnoreSelf) const
{
	TArray<FHitResult> OutResults;

	// 只需初始化一次
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	if (bIgnoreSelf)
	{
		ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	}

	EDrawDebugTrace::Type DrawDebugType = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	for (const TSharedPtr<FGameplayAbilityTargetData>& Data : TargetDataHandle.Data)
	{
		const FVector StartLocation = Data->GetOrigin().GetTranslation();
		const FVector EndLocation = Data->GetEndPoint();

		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::SphereTraceMultiForObjects(
			this,
			StartLocation,
			EndLocation,
			SphereSweepRadius,
			ObjectTypes,
			false,
			ActorsToIgnore,
			DrawDebugType,
			HitResults,
			false
		);

		if (HitResults.Num() > 0)
		{
			OutResults.Append(HitResults);
		}
	}
	return OutResults;
}
