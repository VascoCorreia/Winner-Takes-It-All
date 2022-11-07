// Fill out your copyright notice in the Description page of Project Settings.


#include "WI_AttackMenu.h"
#include "projectPlayerController.h"
#include "projectCharacter.h"

void UWI_AttackMenu::printToViewport()
{
	
}

void UWI_AttackMenu::clickedAttack_Implementation(AActor* playerAttacking, AActor* playerBeingAttacked)
{
	
	//Cast<AprojectCharacter>(playerBeingAttacked)->currentHealth -= damageAmount;

	//UE_LOG(LogTemp,Warning,TEXT("%s attacks %s for %f damage"), *playerAttacking->GetName(), *playerBeingAttacked->GetName(), damageAmount);
}

