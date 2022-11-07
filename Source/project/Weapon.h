// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

// TODO: Used temporarly to be able to identify weapon types in switch cases.
// Remove all related logic when/if the weapon will be defined in blueprint alone
UENUM(BlueprintType)
enum class EWeaponName : uint8
{
	None		UMETA(DisplayName = "None"),
	Club		UMETA(DisplayName = "Club"),
	Knife		UMETA(DisplayName = "Knife"),
	Bomb		UMETA(DisplayName = "Bomb"),
	Brick		UMETA(DisplayName = "Brick"),
};

UCLASS()
class PROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* OnEquipSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	UFUNCTION(Client, Reliable)
	void Equip(class AprojectCharacter* Character);

	/* Setters and Getters */
	FORCEINLINE EWeaponName GetName() { return Name; }

	FORCEINLINE float GetDamage() { return Damage; }

	FORCEINLINE float GetRange() { return Range; }

	FORCEINLINE float GetDropChance() { return DropChance; }

	FORCEINLINE UTexture2D* GetImage() { return Image; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters")
		EWeaponName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters")
		float Damage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters")
		float Range = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Parameters")
		float DropChance = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
		UTexture2D* Image = nullptr;

};
