// Fill out your copyright notice in the Description page of Project Settings.


#include "WI_AttackMenu.h"
#include "projectGameState.h"
#include "UI/MyUI_InventorySlotBase.h"
#include "Inventory.h"
#include "projectCharacter.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

bool UWI_AttackMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	UpdateInventorySlots();
	UpdateChosenWeapon();

	/* Bind delegates */
	if (InventorySlot0 != nullptr)
	{
		InventorySlot0->OnInventorySlotClicked.BindUObject(this, &UWI_AttackMenu::HandleWeaponSlotClicked);
	}
	if (InventorySlot1 != nullptr)
	{
		InventorySlot1->OnInventorySlotClicked.BindUObject(this, &UWI_AttackMenu::HandleWeaponSlotClicked);
	}
	if (InventorySlot2 != nullptr)
	{
		InventorySlot2->OnInventorySlotClicked.BindUObject(this, &UWI_AttackMenu::HandleWeaponSlotClicked);
	}
	if (InventorySlot3 != nullptr)
	{
		InventorySlot3->OnInventorySlotClicked.BindUObject(this, &UWI_AttackMenu::HandleWeaponSlotClicked);
	}

	AprojectCharacter* Character = GetCharacter();
	if (Character != nullptr)
	{
		Character->OnWeaponEquipped.BindUObject(this, &UWI_AttackMenu::HandleWeaponEquippedByCharacter);
	}

	return true;
}

void UWI_AttackMenu::HandleWeaponSlotClicked(int32 SlotIndex)
{
	AprojectCharacter* Character = GetCharacter();
	if (Character != nullptr)
	{
		Character->HandleInventorySlotClicked(SlotIndex);
	}
}

void UWI_AttackMenu::HandleWeaponEquippedByCharacter()
{
	UpdateChosenWeapon();
}

void UWI_AttackMenu::UpdateInventorySlots()
{
	if (!InventorySlot0 || !InventorySlot1 || !InventorySlot2 || !InventorySlot3) return;

	AprojectCharacter* Character = GetCharacter();
	if (Character != nullptr)
	{
		InventorySlot0->UpdateSlot(Character->Inventory->GetItemAtIndex(0), 0, Character->Inventory);
		InventorySlot1->UpdateSlot(Character->Inventory->GetItemAtIndex(1), 1, Character->Inventory);
		InventorySlot2->UpdateSlot(Character->Inventory->GetItemAtIndex(2), 2, Character->Inventory);
		InventorySlot3->UpdateSlot(Character->Inventory->GetItemAtIndex(3), 3, Character->Inventory);
	}
}

void UWI_AttackMenu::UpdateChosenWeapon()
{
	if (!ChosenWeaponImage || !ChosenWeaponName) return;

	AprojectCharacter* Character = GetCharacter();
	if (Character != nullptr)
	{
		ChosenWeaponImage->SetBrushFromTexture(Character->GetEquippedWeapon()->GetImage());
		ChosenWeaponImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ChosenWeaponName->SetText(UEnum::GetDisplayValueAsText(Character->GetEquippedWeapon()->GetName()));
	}
}

AprojectCharacter* UWI_AttackMenu::GetCharacter()
{
	UWorld* world = GetWorld();
	if (world == nullptr) return nullptr;

	APlayerController* pc = dynamic_cast<APlayerController*>(world->GetFirstPlayerController());
	if (pc == nullptr) return nullptr;

	AprojectCharacter* character = dynamic_cast<AprojectCharacter*>(pc->GetPawn());
	if (character == nullptr) return nullptr;

	return character;
}
