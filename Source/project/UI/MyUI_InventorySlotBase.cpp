// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUI_InventorySlotBase.h"

#include "Components/Button.h"

bool UMyUI_InventorySlotBase::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (SlotButton != nullptr && IsClickable)
	{
		SlotButton->OnClicked.AddDynamic(this, &UMyUI_InventorySlotBase::OnSlotButtonClicked);
	}

	return true;
}

void UMyUI_InventorySlotBase::OnSlotButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("INVENTORY_SLOT_BASE: ON_SLOT_BUTTON_CLICKED: Slot button clicked. Index : %i; Is OnInventorySlotClicked bound? %i"), ItemIndex, OnInventorySlotClicked.IsBound());
	OnInventorySlotClicked.ExecuteIfBound(ItemIndex);
}
