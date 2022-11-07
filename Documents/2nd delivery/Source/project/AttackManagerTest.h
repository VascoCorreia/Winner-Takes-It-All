// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AttackManagerTest.generated.h"

struct FAttack
{
	AActor* playerAttacking;
	AActor* playerBeingAttacked;
};

UCLASS()
class PROJECT_API UAttackManagerTest : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	static TArray<FAttack> attackQueue;

	UFUNCTION(BlueprintCallable)
	static void addAttackToQueue(AActor* playerAttacking, AActor* playerBeingAttacked);

	static void clearAttackQueue();

	static void ExecuteAttackQueue();

	static void DoDamage(AActor* playerBeingAttacked, float damageAmount);
};

