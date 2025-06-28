// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CrunchPlayerController.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class ACrunchPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// only called  on the server
	void OnPossess(APawn* NewPawn) override;

	// only called on the client
	void AcknowledgePossession(APawn* NewPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effect")
	TObjectPtr<APlayerCharacter> PlayerCharacter;
};
