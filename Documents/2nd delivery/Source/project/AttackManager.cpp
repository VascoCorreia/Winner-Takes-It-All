//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "AttackManager.h"
//#include "projectCharacter.h"
//
////Where should i I put this attack queue?
//TArray<FAttack> AttackManager::attackQueue;
//
//AttackManager::AttackManager()
//{
//}
//
//AttackManager::~AttackManager()
//{
//}
//
////this class will have a function too add attacks to a queue that are later executed 
////when the game reaches the afterCombatPhase. 
//
////Called when a player clicks Attack Button on Attack Menu
//void addAttackToQueue_Implementation(AActor* playerAttacking, AActor* playerBeingAttacked) 
//{
//	FAttack newAttack;
//
//	newAttack.playerAttacking = playerAttacking;
//	newAttack.playerBeingAttacked = playerBeingAttacked;
//
//	AttackManager::attackQueue.Emplace(newAttack);
//}
//
////Called at the end of AfterCombatPhase
//void clearAttackQueue()
//{
//	AttackManager::attackQueue.Empty();
//}
//
//void ApplyDamage(AActor* playerBeingAttacked, float damageAmount)
//{
//	Cast<AprojectCharacter>(playerBeingAttacked)->currentHealth -= damageAmount;
//}
//
////Does this create a copy of the attack queue? Should I pass it by reference
////This is intended to be called when the game arrives at AfterCombatPhase
//void ExecuteAttackQueue()
//{
//	for (int i = 0; i<= AttackManager::attackQueue.Num(); i++)
//	{
//		ApplyDamage(AttackManager::attackQueue[i].playerBeingAttacked, 1.0f);
//	}
//}
//
