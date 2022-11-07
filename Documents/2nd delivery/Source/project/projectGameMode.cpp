// Copyright Epic Games, Inc. All Rights Reserved.

#include "projectGameMode.h"
#include "projectPlayerController.h"
#include "projectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "WI_AttackMenu.h"
#include <GameFramework/GameModeBase.h>
#include "projectGameState.h"

AprojectGameMode::AprojectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AprojectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickEnabled(true);

}
