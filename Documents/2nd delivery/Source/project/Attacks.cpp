//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "Attacks.h"
//#include "projectCharacter.h"
//
//// Sets default values
//AAttacks::AAttacks()
//{
//	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//	attackQueue.Empty();
//}
//
//// Called when the game starts or when spawned
//void AAttacks::BeginPlay()
//{
//	Super::BeginPlay();
//
//}
//
//// Called every frame
//void AAttacks::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
//
//void AAttacks::addAttackToQueue_Implementation(AActor* playerAttacking, AActor* playerBeingAttacked)
//{
//	FAttack newAttack;
//
//	newAttack.playerAttacking = playerAttacking;
//	newAttack.playerBeingAttacked = playerBeingAttacked;
//
//	attackQueue.Emplace(newAttack);
//}
//
//void AAttacks::clearAttackQueue()
//{
//	attackQueue.Empty();
//}
//
//
//void AAttacks::ApplyDamage_Implementation(AActor* playerBeingAttacked, float damageAmount)
//{
//	Cast<AprojectCharacter>(playerBeingAttacked)->currentHealth -= damageAmount;
//}
//
//void AAttacks::ExecuteAttackQueue()
//{
//	for (int i = 0; i <= attackQueue.Num(); i++)
//	{
//		ApplyDamage(attackQueue[i].playerBeingAttacked, 1.0f);
//	}
//}
//
//
//
