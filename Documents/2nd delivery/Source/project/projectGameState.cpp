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
#include "AttackManagerTest.h"


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

	gamemodeState = EGameModeState::InMovementPhase;

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

	//Replicate current health.
	DOREPLIFETIME(AprojectGameState, phaseTimer);
	DOREPLIFETIME(AprojectGameState, gamemodeState);
}

void AprojectGameState::InMovementPhase(float DeltaTime)
{
	if (HasAuthority())
	{
		disableMovement();
	}

	if (phaseTimer >= 0 && HasAuthority())
	{
		phaseTimer -= DeltaTime;

		return;
	}

	//bool to reset some values on the movement phase and make it only run once. Spaaaagueeettiiiii
	isFirstTickOfMovementPhase = true;

	if (HasAuthority())
	{
		phaseTimer = 5;
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
				UE_LOG(LogTemp, Warning, TEXT("On Combat Phase"));
				playerController->canMove = false;
			}
		}
	}
	//Sets player can move to false if we are on the combat phase


	if (PC && PC->attackMenu)
	{
		PC->attackMenu->SetIsEnabled(true);
	}


	if (phaseTimer >= 0 && HasAuthority())
	{
		phaseTimer -= DeltaTime;
		return;
	}

	if (HasAuthority())
	{
		phaseTimer = 5;
		gamemodeState = EGameModeState::AfterCombatPhase;
	}

}

void AprojectGameState::AfterCombatPhase()
{
	UE_LOG(LogTemp, Warning, TEXT("After Combat Phase"));

	//Call AttackManager::Exec
	if (HasAuthority())
	{
		UAttackManagerTest::ExecuteAttackQueue();
		UAttackManagerTest::clearAttackQueue();
	}

	if (HasAuthority())
	{
		gamemodeState = EGameModeState::InMovementPhase;
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

					//resets the stamina of a player in the beginning of movement phase
					Cast<AprojectCharacter>(playerController->GetPawn())->currentStamina = 3.0f;

					UE_LOG(LogTemp, Warning, TEXT("On Movement Phase"));

					//Disables attack menu on movement phase
					if (playerController->attackMenu)
					{
						playerController->attackMenu->SetIsEnabled(false);
					}
				}
			}

			//If stamina is lower than 0 player cant move
			if (playerController)
			{
				if (Cast<AprojectCharacter>(playerController->GetPawn())->currentStamina <= 0.0f)
				{
					playerController->canMove = false;
				}
			}
		}
	}
	isFirstTickOfMovementPhase = false;
}

#pragma optimize ("", off)