// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventory::UInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	/*UE_LOG(LogTemp, Warning, TEXT("INVENTORY: Inventory Start"));*/
}

// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the array with empty slots
	if (NumberOfSlots > 0)
	{
		for (int32 i = 0; i < NumberOfSlots - 1; i++)
		{
			Items.Emplace(FInventoryItem());
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Number of slots: %i"), Items.Num());
}

// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventory::IsSlotEmpty(int32 index) const
{
	if (Items.Num() <= 0) return false;
	/*UE_LOG(LogTemp, Warning, TEXT("INVENTORY: IS_SLOT_EMPTY: Index: %i, isValidIndex: %s , is Amount less than 1: %s, actual Amount: %i"), 
		index,
		Items.IsValidIndex(index) ? *FString("True") : *FString("False"),
		Items[index].Amount <= 0 ? *FString("True") : *FString("False"),
		Items[index].Amount
		);*/
	return !Items.IsValidIndex(index) || Items[index].Amount <= 0;

	// TODO: Version when Item will be finally added
	/*return !Items.IsValidIndex(index)
		|| Items[index].Amount <= 0
		|| (Items[index].Item != nullptr && Items[index].Item->GetName().Len() <= 0);*/
}

FInventoryItem UInventory::GetItemAtIndex(int32 index) const
{
	if (!Items.IsValidIndex(index))
	{
		UE_LOG(LogTemp, Error, TEXT("INVENTORY: Called UInventory::GetItemAtIndex() with invalid index parameter %i"), index);
		return FInventoryItem();
	}
	//UE_LOG(LogTemp, Warning, TEXT("INVENTORY: GET_ITEM_AT_INDEX: Item at index %i : %s"), index, *Items[index].ToString());
	return Items[index];
}

TArray<FInventoryItem> UInventory::GetItems()
{
	//UE_LOG(LogTemp, Warning, TEXT("Getting items: %s"), *PrintItems());
	return Items;
}

void UInventory::AddItem(FInventoryItem item)
{
	int32 index = Items.IndexOfByPredicate([item](const FInventoryItem& itemInInventory) {
		return itemInInventory.Name == item.Name;
	});

	/*int32 Index = Items.IndexOfByPredicate([item](const FInventoryItem& itemInInventory) {
		if (itemInInventory.Item == nullptr || item.Item == nullptr)
		{
			return false;
		}
		return itemInInventory.Item->GetName() == item.Item->GetName();
		}
	);*/

	//UE_LOG(LogTemp, Warning, TEXT("INVENTORY: ADD_ITEM: Index of found item: %i"), index);
	// if there is no item of the specified type, try to add a new one
	if (index == INDEX_NONE)
	{
		bool ItemWasAdded = false;
		for (int32 i = 0; i < Items.Num(); i++)
		{
			if (IsSlotEmpty(i))
			{
				Items[i] = item;
				ItemWasAdded = true;

				FString itemName = UEnum::GetDisplayValueAsText(item.Name).ToString();
				//UE_LOG(LogTemp, Warning, TEXT("INVENTORY: Add new item %s : %i"), 
					//*itemName, item.Amount);
				break;
			}
		}

		if (!ItemWasAdded)
		{
			UE_LOG(LogTemp, Warning, TEXT("INVENTORY: All slots in the inventory are taken"));
		}
	}
	// it there is already an item with the specified type in inventory, update it with the provided amount
	else
	{
		Items[index].Amount += item.Amount;

		FString itemName = UEnum::GetDisplayValueAsText(Items[index].Name).ToString();
		UE_LOG(LogTemp, Warning, TEXT("Updated item %s. New amount = %i"), *itemName, Items[index].Amount);
		
		/*if (Items[Index].Item != nullptr)
		{
			Items[Index].Amount += item.Amount;
			UE_LOG(LogTemp, Warning, TEXT("INVENTORY: Updated item %s. New amount = %i"), *FString(Items[Index].Name), Items[Index].Amount);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("INVENTORY: Item property at index %i is 'nullptr'. Expeted item.Item property to be set."), Index);
		}*/
	}
}

FString UInventory::PrintItems()
{
	FString itemsStr = TEXT("INVENTORY: ");
	for (auto& item : Items)
	{
		FString itemName = UEnum::GetDisplayValueAsText(item.Name).ToString();

		itemsStr += TEXT("\nAmount: ");
		itemsStr += FString::FromInt(item.Amount);
		itemsStr += TEXT(", Name: ");
		itemsStr += itemName;
		//itemsStr += item.Item == nullptr ? "NULLPTR" : item.Item->GetName();
		itemsStr += TEXT("\n");
	}
	return itemsStr;
}

void UInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UInventory, Items);
}

