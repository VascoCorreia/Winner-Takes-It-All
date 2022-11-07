// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Attacks.h"
#include "projectGameState.generated.h"

UENUM(BlueprintType)
enum EGameModeState {
	InMovementPhase,
	InCombatPhase,
	AfterCombatPhase,
};

UCLASS()
class PROJECT_API AprojectGameState : public AGameState
{
	GENERATED_BODY()

public:
	AprojectGameState();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InMovementPhase(float DeltaTime);
	void InCombatPhase(float DeltaTime);

	//GAMESTATE EXISTS ON THE SERVER AND IS REPLICATED TO ALL CLIENTS SO MAYBE I CANT CALL
	//RPC HERE 

	void AfterCombatPhase();
 
	UFUNCTION(Server, Reliable)
		void disableMovement();

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Phases")
		float phaseTimer = 5.0f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Phases")
		TEnumAsByte<EGameModeState> gamemodeState;

private:

	class AprojectPlayerController* PC = nullptr;

	class AprojectPlayerState* playerState = nullptr;

	bool isFirstTickOfMovementPhase;

};
