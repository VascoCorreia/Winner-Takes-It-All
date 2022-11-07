// Fill out your copyright notice in the Description page of Project Settings.


#include "projectGameState.h"
#include "projectPlayerController.h"
#include "projectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "WI_AttackMenu.h"
#include "projectPlayerState.h"
#include <GameFramework/Controller.h>
#include <Net/UnrealNetwork.h>
#include <Kismet/GameplayStatics.h>
#include "DamageManager.h"
#include "GameFramework/CharacterMovementComponent.h"

AprojectGameState::AprojectGameState() {
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickEnabled(true);
}

void AprojectGameState::BeginPlay()
{
	Super::BeginPlay();

	PC = (AprojectPlayerController*)GetWorld()->GetFirstPlayerController();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	gamemodeState = EGameModeState::InMovementPhase;

	//gamemodeState = World->GetMapName().Contains("Lobby") ?
	//	EGameModeState::InLobby :
	//	EGameModeState::InMovementPhase;

	UE_LOG(LogTemp, Warning, TEXT("GAME STATE: BEGIN PLAY, setting up the initial gamemodeSate"));

	//attackManager = Cast<AAttacks>(UGameplayStatics::GetActorOfClass(GetWorld(), AAttacks::StaticClass()));
	//playerState = Cast<AprojectPlayerState>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetPlayerState());
}

void AprojectGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (gamemodeState)
	{
	case EGameModeState::InMovementPhase:
		InMovementPhase(DeltaTime);
		break;
	case EGameModeState::InCombatPhase:
		InCombatPhase(DeltaTime);
		break;
	case EGameModeState::AfterCombatPhase:
		AfterCombatPhase();
		break;
	}
}

void AprojectGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AprojectGameState, movementPhaseTimer);
	DOREPLIFETIME(AprojectGameState, combatPhaseTimer);
	DOREPLIFETIME(AprojectGameState, gamemodeState);
	DOREPLIFETIME(AprojectGameState, bIsSetup);
}

void AprojectGameState::InMovementPhase(float DeltaTime)
{

	if (HasAuthority())
	{
		disableMovement();
	}

	if (movementPhaseTimer >= 0 && HasAuthority())
	{
		movementPhaseTimer -= DeltaTime;

		return;
	}

	//bool to reset some values on the movement phase and make it only run once. Spaaaagueeettiiiii
	isFirstTickOfMovementPhase = true;

	if (HasAuthority())
	{
		movementPhaseTimer = 15.f;
		gamemodeState = EGameModeState::InCombatPhase;
	}
}

void AprojectGameState::InCombatPhase(float DeltaTime)
{

	auto playerIterators = GetWorld()->GetPlayerControllerIterator();
	for (auto currentPlayerIterator = playerIterators; currentPlayerIterator; ++currentPlayerIterator)
	{
		AprojectPlayerController* playerController = Cast<AprojectPlayerController>(currentPlayerIterator->Get());

		if (playerController)
		{
			if (playerController->canMove)
			{
				playerController->canMove = false;
			}
		}
	}
	//Sets player can move to false if we are on the combat phase


	//if (PC && PC->attackMenu)
	//{
	//	PC->attackMenu->SetIsEnabled(true);
	//}


	if (combatPhaseTimer >= 0 && HasAuthority())
	{
		combatPhaseTimer -= DeltaTime;
		return;
	}

	if (HasAuthority())
	{
		combatPhaseTimer = 5;
		gamemodeState = EGameModeState::AfterCombatPhase;
	}

}

void AprojectGameState::AfterCombatPhase()
{
	if (HasAuthority())
	{
		gamemodeState = EGameModeState::InMovementPhase;
	}
}

void AprojectGameState::SetupGameStateForLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("GAME STATE: SetupGameStateForLevel, about to check if has authority"));
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("GAME STATE: SetupGameStateForLevel,  has authority, setting inMovement phase"));
		gamemodeState = EGameModeState::InMovementPhase;
		bIsSetup = true;
	}
}

void AprojectGameState::disableMovement_Implementation()
{
	auto playerIterators = GetWorld()->GetPlayerControllerIterator();
	for (auto currentPlayerIterator = playerIterators; currentPlayerIterator; ++currentPlayerIterator)
	{
		AprojectPlayerController* playerController = Cast<AprojectPlayerController>(currentPlayerIterator->Get());

		if (playerController)
		{
			if (isFirstTickOfMovementPhase)
			{
				if (!(playerController->canMove))
				{
					playerController->canMove = true;
					Cast<AprojectCharacter>(playerController->GetPawn())->FindComponentByClass<UCharacterMovementComponent>()->MaxWalkSpeed = 600.f;

					//resets the stamina of a player in the beginning of movement phase
					Cast<AprojectCharacter>(playerController->GetPawn())->currentStamina = 10.0f;
						
					////Disables attack menu on movement phase
					//if (playerController->attackMenu)
					//{
					//	playerController->attackMenu->SetIsEnabled(false);
					//}
				}
			}

			//If stamina is lower than 0 player cant move
			if (playerController)
			{
				if (Cast<AprojectCharacter>(playerController->GetPawn())->currentStamina <= 0.0f)
				{
					playerController->canMove = false;
					Cast<AprojectCharacter>(playerController->GetPawn())->FindComponentByClass<UCharacterMovementComponent>()->MaxWalkSpeed = 0.f;
				}
			}
		}
	}
	isFirstTickOfMovementPhase = false;
}

#pragma optimize ("", off)