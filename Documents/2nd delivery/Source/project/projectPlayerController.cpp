// Copyright Epic Games, Inc. All Rights Reserved.

#include "projectPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "CameraManager.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "projectCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "WI_AttackMenu.h"
#include "projectGameMode.h"
#include "projectGameState.h"
#include <Kismet/GameplayStatics.h>
#include "projectPlayerState.h"
#include <Net/UnrealNetwork.h>

AprojectPlayerController::AprojectPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	canMove = true;
}

void AprojectPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AprojectPlayerController, canMove);

}

void AprojectPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AprojectPlayerController::SetupInputComponent()
{
	// set up game play key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AprojectPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AprojectPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("RightClickActor", IE_Pressed, this, &AprojectPlayerController::ClickedActor);

	//InputComponent->BindAction("SnapCameraToPlayer", IE_Pressed, this, &UCameraManager::CameraSnapToPlayer);
}

void AprojectPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AprojectCharacter* MyPawn = Cast<AprojectCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AprojectPlayerController::SetNewMoveDestination_Implementation(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();	

	if (MyPawn && canMove)
	{
		
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AprojectPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AprojectPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AprojectPlayerController::ClickedActor()
{
	FHitResult hitResult;
	
	gameState = (AprojectGameState*)GetWorld()->GetGameState();

	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, true, hitResult);
		
	/*This comment below will stop the menu from showing when player click on their own characters
	&& !(hitResult.GetActor()->GetOwner())*/
	if (hitResult.GetActor()->ActorHasTag("Player"))
	{
		
		attackMenu = CreateWidget<UUserWidget>(GetWorld(), *attackMenuToSpawn, "attackUI");

		//store the clicked actor
		storeClickedPlayer = hitResult.GetActor();
		
		//If we are on movement phase attack menu is disabled if we are on combat phase it is enabled.
		if (gameState)
		{
			if (gameState->gamemodeState == EGameModeState::InMovementPhase)
			{
				attackMenu->SetIsEnabled(false);
			}
		}

		//Changes Widget player name text to the name of actor (needs to change to player name)
		if (UWI_AttackMenu* attackMenuInstance = Cast<UWI_AttackMenu>(attackMenu))	
		{
			attackMenuInstance->playerName->SetText(FText::FromString((TEXT("%s"), *hitResult.GetActor()->GetName())));
		}

		if (attackMenu)
			attackMenu->AddToViewport();
	}
	else
	{
		if (attackMenu)
			attackMenu->RemoveFromViewport();
	}
}
