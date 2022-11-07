// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackManagerTest.h"
#include "projectCharacter.h"


TArray<FAttack> UAttackManagerTest::attackQueue;

void UAttackManagerTest::addAttackToQueue(AActor* playerAttacking, AActor* playerBeingAttacked)
{
	FAttack newAttack;

	newAttack.playerAttacking = playerAttacking;
	newAttack.playerBeingAttacked = playerBeingAttacked;

	UE_LOG(LogTemp, Warning, TEXT("%s attacking %s"), *playerAttacking->GetName(), *playerBeingAttacked->GetName())

		UAttackManagerTest::attackQueue.Emplace(newAttack);
}

//Called at the end of AfterCombatPhase
void UAttackManagerTest::clearAttackQueue()
{
	UAttackManagerTest::attackQueue.Empty();
}

void UAttackManagerTest::DoDamage(AActor* playerBeingAttacked, float damageAmount)
{
	Cast<AprojectCharacter>(playerBeingAttacked)->currentHealth -= damageAmount;
}

//Does this create a copy of the attack queue? Should I pass it by reference
//This is intended to be called when the game arrives at AfterCombatPhase
void UAttackManagerTest::ExecuteAttackQueue()
{
	for (int i = 0; i < UAttackManagerTest::attackQueue.Num(); i++)
	{
		UAttackManagerTest::DoDamage(UAttackManagerTest::attackQueue[i].playerBeingAttacked, 1.0f);
	}
}