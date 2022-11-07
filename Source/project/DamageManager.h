// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageManager.generated.h"

USTRUCT()
struct FAttack
{
	GENERATED_BODY()

		UPROPERTY()
		AprojectCharacter* playerAttacking;
	UPROPERTY()
		AprojectCharacter* playerBeingAttacked;
};


UCLASS()
class PROJECT_API ADamageManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADamageManager();

	UPROPERTY(Replicated);
	TArray<FAttack> attackQueue;

	class AprojectGameState* gameState = nullptr;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void addAttackToQueue(AprojectCharacter* playerAttacking, AprojectCharacter* playerBeingAttacked);

	void clearAttackQueue();

	void ExecuteAttackQueue();

	void DoDamage(AActor* playerBeingAttacked, float damageAmount);

	UFUNCTION(BlueprintCallable)
		bool IsObjectBetweenPlayers(AActor* player1, AActor* player2);

	UFUNCTION(BlueprintCallable)
		bool IsPlayerInRange(class AprojectCharacter* player1, AActor* player2);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
