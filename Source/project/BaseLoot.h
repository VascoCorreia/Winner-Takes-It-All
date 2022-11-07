// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "Weapon.h"
#include "BaseLoot.generated.h"

UCLASS()
class PROJECT_API ABaseLoot : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseLoot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Loot Params")
		EWeaponName Name;

	UPROPERTY(EditAnywhere, Category = "Loot Params")
		int32 Amount;

	UPROPERTY(EditAnywhere, Category = "Loot Params")
		UTexture2D* Image = nullptr;

	UPROPERTY(EditAnywhere, Category = "Loot Params")
		TSubclassOf<AWeapon> Item = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USphereComponent* SphereComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
