// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CrunchCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/CrunchAbilitySystemComponent.h"
#include "GAS/CrunchAttributeSet.h"
#include "Widgets/OverHeadStatsGuage.h"

// Sets default values
ACrunchCharacter::ACrunchCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CrunchAbilitySystemComponent = CreateDefaultSubobject<UCrunchAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	CrunchAttributeSet = CreateDefaultSubobject<UCrunchAttributeSet>(TEXT("AttributeSet"));

	// Initialize the OverHeadWidgetComponent
	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidgetComponent"));
	OverHeadWidgetComponent->SetupAttachment(GetRootComponent());
}

void ACrunchCharacter::ServerSideInit()
{
	CrunchAbilitySystemComponent->InitAbilityActorInfo(this, this);
	CrunchAbilitySystemComponent->ApplyInitialEffects();
}

void ACrunchCharacter::ClientSideInit()
{
	CrunchAbilitySystemComponent->InitAbilityActorInfo(this, this);
}

bool ACrunchCharacter::IsLocallyControlledByPlayer() const
{
	return GetController() && GetController()->IsLocalPlayerController();
}

void ACrunchCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (NewController && !NewController->IsPlayerController())
	{
		ServerSideInit();
	}
}

// Called when the game starts or when spawned
void ACrunchCharacter::BeginPlay()
{
	Super::BeginPlay();

	ConfigureOverHeadWidgetComponent();
}

// Called every frame
void ACrunchCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACrunchCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ACrunchCharacter::GetAbilitySystemComponent() const
{
	return CrunchAbilitySystemComponent;
}

void ACrunchCharacter::ConfigureOverHeadWidgetComponent()
{
	if (!OverHeadWidgetComponent)
	{
		return;
	}

	if (IsLocallyControlledByPlayer())
	{
		OverHeadWidgetComponent->SetHiddenInGame(true);
		return;
	}

	UOverHeadStatsGuage* OverHeadStatsGuage = Cast<UOverHeadStatsGuage>(OverHeadWidgetComponent->GetUserWidgetObject());

	if (OverHeadStatsGuage)
	{
		OverHeadStatsGuage->ConfigureWithASC(GetAbilitySystemComponent());
		OverHeadWidgetComponent->SetHiddenInGame(false);
	}
}