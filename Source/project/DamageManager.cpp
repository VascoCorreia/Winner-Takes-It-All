// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageManager.h"
#include "projectCharacter.h"
#include "projectGameState.h"
#include "Kismet/GameplayStatics.h"
#include <Net/UnrealNetwork.h>
#include "DrawDebugHelpers.h"

// Sets default values
ADamageManager::ADamageManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADamageManager::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		gameState = Cast<AprojectGameState>(UGameplayStatics::GetGameState(GetWorld()));
	}
}

// Called every frame
void ADamageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (gameState && HasAuthority())
	{
		if (gameState->gamemodeState == 2)
		{
			ExecuteAttackQueue();
			clearAttackQueue();
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.
#pragma optimize ("", off)

void ADamageManager::addAttackToQueue_Implementation(AprojectCharacter* playerAttacking, AprojectCharacter* playerBeingAttacked)
{
	FAttack newAttack;

	newAttack.playerAttacking = playerAttacking;
	newAttack.playerBeingAttacked = playerBeingAttacked;

	UE_LOG(LogTemp, Warning, TEXT("%s attacking %s"), *playerAttacking->GetName(), *playerBeingAttacked->GetName())

		attackQueue.Emplace(newAttack);
}

//Called at the end of AfterCombatPhase
void ADamageManager::clearAttackQueue()
{
	attackQueue.Empty();
}

void ADamageManager::DoDamage(AActor* playerBeingAttacked, float damageAmount)
{
	if (AprojectCharacter* test = Cast<AprojectCharacter>(playerBeingAttacked))
	{
		test->currentHealth -= damageAmount;
	}
	//Cast<AprojectCharacter>(playerBeingAttacked)->currentHealth -= damageAmount;
}

//Does this create a copy of the attack queue? Should I pass it by reference
//This is intended to be called when the game arrives at AfterCombatPhase
void ADamageManager::ExecuteAttackQueue()
{
	for (int i = 0; i < attackQueue.Num(); i++)
	{
		ADamageManager::DoDamage(attackQueue[i].playerBeingAttacked, attackQueue[i].playerAttacking->GetEquippedWeapon()->GetDamage());
	}
}

bool ADamageManager::IsObjectBetweenPlayers(AActor* player1, AActor* player2)
{
	FHitResult hitResult;

	const static FName Tag = TEXT("DebugTrace");
	GetWorld()->DebugDrawTraceTag = Tag;
	FCollisionQueryParams TraceParams(Tag);

	GetWorld()->LineTraceSingleByChannel(hitResult, player1->GetActorLocation(), player2->GetActorLocation(), ECC_GameTraceChannel1, TraceParams);

	if (hitResult.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *hitResult.GetActor()->GetName());
		return true;
	}
	else
	{
		return false;
	}
}

#pragma optimize ("", on)

bool ADamageManager::IsPlayerInRange(AprojectCharacter* player1, AActor* player2)
{
	if (player1->GetEquippedWeapon()->GetRange() >= FVector::Dist(player1->GetActorLocation(), player2->GetActorLocation()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ADamageManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADamageManager, attackQueue);
}