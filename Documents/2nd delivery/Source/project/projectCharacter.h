// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "GameFramework/Character.h"
#include "projectCharacter.generated.h"

class UInventory;

UCLASS(Blueprintable)
class AprojectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AprojectCharacter();

	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UInventory* Inventory = nullptr;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
		float currentHealth;

	UPROPERTY(Replicated, BlueprintReadWrite)
		float currentStamina = 3.0f;

	UFUNCTION(NetMulticast, Reliable)
		void replicateRotation(FRotator rotation);

	class AGameState* gameState = nullptr;

	class AprojectPlayerState* playerState = nullptr;

	// TODO: Are those needed still?
	USkeletalMeshComponent* meshComponent = nullptr;
	UCapsuleComponent* trigger = nullptr;

	UFUNCTION()
		void AddItemToInventory(FInventoryItem item);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void OnAddItemToInventory(FInventoryItem item);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;
};

