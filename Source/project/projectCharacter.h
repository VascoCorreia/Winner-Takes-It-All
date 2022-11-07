// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "GameFramework/Character.h"
#include "projectCharacter.generated.h"

class UMyUI_InventorySlotBase;

DECLARE_DELEGATE(FOnWeaponEquippedDelegate)
UCLASS(Blueprintable)
class AprojectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AprojectCharacter();

	virtual void BeginPlay() override;

	// Event Delegate that can be used by another class
	FOnWeaponEquippedDelegate OnWeaponEquipped;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UInventory* Inventory = nullptr;

	UFUNCTION(Client, Reliable)
		void AddItemToInventory(FInventoryItem item);

	UFUNCTION(NetMulticast, Reliable)
		void PlayerDied();

	// Probably should be an RPC executed on the Client like AddItemToInventory
	UFUNCTION()
		void HandleInventorySlotClicked(int32 SlotIndex);

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
		float currentHealth;

	UPROPERTY(Replicated, BlueprintReadWrite)
		float currentStamina = 10.0f;

	UFUNCTION(NetMulticast, Reliable)
		void replicateRotation(FRotator rotation);

	UPROPERTY(BlueprintReadOnly)
	class AprojectGameState* gameState = nullptr;

	class AprojectPlayerState* playerState = nullptr;

	class UprojectGameInstance* GameInstance = nullptr;

	// TODO: Are those needed still?
	USkeletalMeshComponent* meshComponent = nullptr;

	UCapsuleComponent* trigger = nullptr;

	void SpawnWeaponRangeDecal(AWeapon* equippedWeapon);

	void SpawnStaminaRangeDecal(); 
	
	void UpdateStaminaRangeDecal();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	void SetEquippedWeapon(AWeapon* Weapon);

	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }

protected:

	UFUNCTION(Client, Reliable)
		void OnInventoryUpdate();

	void UpdateInventorySlotsInUI();

	UFUNCTION(Client, Reliable)
		void EquipWeaponAtIndex(int32 InventoryItemIndex);

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
		class AWeapon* EquippedWeapon;

private:
	void SetupProjectCharacterForLevel();

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

	// UI_Game widget
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUserWidget> UserInGameInterfaceWidgetClass;

	class UUserWidget* UserInGameInterfaceWidget;

	USkeletalMeshComponent* characterMesh = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADecalActor> weaponRangeDecalReference;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADecalActor> staminaRangeDecalReference;

	UDecalComponent* staminaRangeDecalComponent = nullptr;

	UCharacterMovementComponent* characterMovementComponent = nullptr;

	// Inventory Slots in UI_Game widegt
	UMyUI_InventorySlotBase* InventorySlot0;
	UMyUI_InventorySlotBase* InventorySlot1;
	UMyUI_InventorySlotBase* InventorySlot2;
	UMyUI_InventorySlotBase* InventorySlot3;
};

