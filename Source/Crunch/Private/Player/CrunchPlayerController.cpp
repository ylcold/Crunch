// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CrunchPlayerController.h"
#include "Player/PlayerCharacter.h"

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
	}
}
