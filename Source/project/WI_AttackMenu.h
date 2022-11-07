// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WI_AttackMenu.generated.h"

class UMyUI_InventorySlotBase;
class UImage;
class UTextBlock;
class AprojectCharacter;

UCLASS()
class PROJECT_API UWI_AttackMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

protected:
	// meta = (BindWidget) makes this property be bound to the variables in the Widget
	// more - https://benui.ca/unreal/ui-bindwidget/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UMyUI_InventorySlotBase* InventorySlot0;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UMyUI_InventorySlotBase* InventorySlot1;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UMyUI_InventorySlotBase* InventorySlot2;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UMyUI_InventorySlotBase* InventorySlot3;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ChosenWeaponImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ChosenWeaponName;

	UFUNCTION()
	void HandleWeaponSlotClicked(int32 SlotIndex);

	UFUNCTION()
	void HandleWeaponEquippedByCharacter();

	UFUNCTION()
	void UpdateInventorySlots();

	UFUNCTION()
	void UpdateChosenWeapon();

	AprojectCharacter* GetCharacter();

};
