//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//
////struct FAttack
////{
////	AActor* playerAttacking;
////	AActor* playerBeingAttacked;
////};
//
//class PROJECT_API AttackManager
//{
//public:
//	AttackManager();
//	~AttackManager();
//
//	static TArray<FAttack> attackQueue;
//
//	UFUNCTION(BlueprintCallable, Server, Reliable)
//	static void addAttackToQueue(AActor* playerAttacking, AActor* playerBeingAttacked);
//
//	static void clearAttackQueue();
//
//	static void ExecuteAttackQueue();
//
//	UFUNCTION(BlueprintCallable, Server, Reliable)
//	static void ApplyDamage(AActor* playerBeingAttacked, float damageAmount);
//};
