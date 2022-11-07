// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

// Sets default values for this component's properties
UInventory::UInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


bool UInventory::IsSlotEmpty(int32 index) const
{
	UE_LOG(LogTemp, Warning, TEXT("Slot at Index: %i -> Type.Len(): %i, Amount: %i. Is it empty? %s"), 
		index, Items[index].Type.Len(), Items[index].Amount,
		*FString((!Items.IsValidIndex(index) || Items[index].Type.Len() <= 0 || Items[index].Amount <= 0 ) ? "True" : "False"));
	
	return !Items.IsValidIndex(index) || Items[index].Type.Len() <= 0 || Items[index].Amount <= 0;
}

FInventoryItem UInventory::GetItemAtIndex(int32 index) const
{
	if (!Items.IsValidIndex(index))
	{
		UE_LOG(LogTemp, Error, TEXT("Called UInventory::GetItemAtIndex() with invalid index parameter %i"), index);
		return FInventoryItem();
	}
	return Items[index];
}

TArray<FInventoryItem> UInventory::GetItems()
{
	UE_LOG(LogTemp, Warning, TEXT("Getting items: %s"), *PrintItems());
	return Items;
}

void UInventory::AddItem(FInventoryItem item)
{
	int32 Index = Items.IndexOfByPredicate([item](const FInventoryItem& itemInInventory) {
		return itemInInventory.Type == item.Type;
		});

	// if there is no item of the specified type, try to add a new one
	if (Index == INDEX_NONE)
	{
		bool ItemWasAdded = false;
		for (int32 i = 0; i < Items.Num(); i++)
		{
			if (IsSlotEmpty(i))
			{
				Items[i] = item;
				ItemWasAdded = true;
				UE_LOG(LogTemp, Warning, TEXT("Add new item %s : %i"), *FString(item.Type), item.Amount);
				break;
			}
		}

		if (!ItemWasAdded)
		{
			UE_LOG(LogTemp, Warning, TEXT("All slots in the inventory are taken"));
		}
	}
	// it there is already an item with the specified type in inventory, update it with the provided amount
	else
	{
		Items[Index].Amount += item.Amount;
		UE_LOG(LogTemp, Warning, TEXT("Updated item %s. New amount = %i"), *FString(Items[Index].Type), Items[Index].Amount);
	}
}

// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the array with empty slots
	if (NumberOfSlots > 0)
	{
		for (int32 i = 0; i < NumberOfSlots; i++)
		{
			Items.Emplace(FInventoryItem());
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Number of slots: %i"), Items.Num());
}

// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FString UInventory::PrintItems()
{
	FString itemsStr = TEXT("");
	for (auto& item : Items)
	{
		itemsStr += TEXT("Amount: ");
		itemsStr += FString::FromInt(item.Amount);
		itemsStr += TEXT(", Name: ");
		itemsStr += item.Type;
		itemsStr += TEXT("\n");
	}
	return itemsStr;
}

