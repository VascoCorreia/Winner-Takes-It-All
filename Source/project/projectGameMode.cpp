// Copyright Epic Games, Inc. All Rights Reserved.

#include "projectGameMode.h"
#include "projectPlayerController.h"
#include "projectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "WI_AttackMenu.h"
#include <GameFramework/GameModeBase.h>
#include "projectGameState.h"
#include "Kismet/GameplayStatics.h"

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

void AprojectGameMode::PostLogin(AprojectPlayerController* newPlayer)
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	//AprojectGameState* GameState = Cast<AprojectGameState>(World->GetGameState());

	AprojectGameState* gameState = Cast<AprojectGameState>(UGameplayStatics::GetGameState(World));

	UE_LOG(LogTemp, Warning, TEXT("GAME_MODE: PostLogin | Found GameState ? %i"), gameState != nullptr);
	//AprojectGameState* GameState = GetGameState();
}
