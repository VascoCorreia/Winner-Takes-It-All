// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../InventoryItem.h"
#include "../Inventory.h"
#include "MyUI_InventorySlotBase.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnInventorySlotClickedDelegate, int32)
UCLASS()
class PROJECT_API UMyUI_InventorySlotBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSlot(FInventoryItem InventoryItem, int32 Index, UInventory *Inventory);

	// Event Delegate that can be used by another class
	FOnInventorySlotClickedDelegate OnInventorySlotClicked;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* SlotButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Parameters")
	int32 ItemIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Parameters")
	bool IsClickable = true;

	// Slot Button clicked internal event
	UFUNCTION()
	void OnSlotButtonClicked();

};
