//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "Attacks.generated.h"
//
////struct FAttack
////{
////	AActor* playerAttacking;
////	AActor* playerBeingAttacked;
////};
//
//UCLASS()
//class PROJECT_API AAttacks : public AActor
//{
//	GENERATED_BODY()
//
//public:
//	// Sets default values for this actor's properties
//	AAttacks();
//
//	TArray<FAttack> attackQueue;
//
//	UFUNCTION(Server, BlueprintCallable, Reliable)
//	void addAttackToQueue(AActor* playerAttacking, AActor* playerBeingAttacked);
//
//	void clearAttackQueue();
//
//	void ExecuteAttackQueue();
//
//	UFUNCTION(BlueprintCallable, Server, Reliable)
//	void ApplyDamage(AActor* playerBeingAttacked, float damageAmount);
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//};
