// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CrunchPlayerController.h"
#include "Player/PlayerCharacter.h"
#include "Widgets/GameplayWidget.h"

void ACrunchPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	PlayerCharacter = Cast<APlayerCharacter>(NewPawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->ServerSideInit();
	}
}

void ACrunchPlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);

	PlayerCharacter = Cast<APlayerCharacter>(NewPawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->ClientSideInit();
		SpawnGameplayWidget();
	}
}

void ACrunchPlayerController::SpawnGameplayWidget()
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	GameplayWidget = CreateWidget<UGameplayWidget>(this, GameplayWidgetClass);
	if (GameplayWidget)
	{
		GameplayWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create GameplayWidget!"));
	}
}
