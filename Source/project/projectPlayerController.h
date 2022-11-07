// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "projectPlayerController.generated.h"

UCLASS()
class AprojectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AprojectPlayerController();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//this variable is controlled in projectGameMode
	UPROPERTY(Replicated)
	bool canMove;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> attackMenuToSpawn;

	UPROPERTY()
		class UUserWidget* attackMenu = nullptr;

	class AprojectGameState* gameState = nullptr;

	UPROPERTY(BlueprintReadWrite)
	AActor* storeClickedPlayer = nullptr;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the given world location. */
	UFUNCTION(Server, Unreliable)
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void ClickedActor();
	void SnapCameraToPlayer();
	void ReduceHealthBy5_Debug();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void RequestDamage(AprojectCharacter* playerAttacking, AprojectCharacter* playerBeingAttacked);
};


