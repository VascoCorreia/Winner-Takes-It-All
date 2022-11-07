// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Weapon.h"
#include "InventoryItem.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	FORCEINLINE FInventoryItem();

	explicit FORCEINLINE FInventoryItem(int32 InAmount, EWeaponName InName);

	explicit FORCEINLINE FInventoryItem(int32 InAmount, EWeaponName InName, UTexture2D* InImage);

	explicit FORCEINLINE FInventoryItem(int32 InAmount, EWeaponName InName, UTexture2D* InImage, TSubclassOf<AWeapon> Item);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AWeapon> Item = nullptr;

	FString ToString() const
	{
		FString itemStr = TEXT("");

		itemStr += TEXT("\nAmount: ");
		itemStr += FString::FromInt(Amount);

		FString itemName = UEnum::GetDisplayValueAsText(Name).ToString();
		itemStr += TEXT(", \nName: ");
		itemStr += itemName;
		
		itemStr += TEXT(",\nItem class: ");
		itemStr += Item == nullptr ? "NULLPTR" : Item->GetName();
		itemStr += TEXT("\n");

		return itemStr;
	}
};

FORCEINLINE FInventoryItem::FInventoryItem() : Amount(0), Name(EWeaponName::None), Image(nullptr)
{
}

FORCEINLINE FInventoryItem::FInventoryItem(const int32 InAmount, EWeaponName InName) : Amount(InAmount), Name(InName), Image(nullptr)
{
}

FORCEINLINE FInventoryItem::FInventoryItem(const int32 InAmount, EWeaponName InName, UTexture2D* InImage) : Amount(InAmount), Name(InName), Image(InImage)
{
}

FORCEINLINE FInventoryItem::FInventoryItem(const int32 InAmount, EWeaponName InName, UTexture2D* InImage, TSubclassOf<AWeapon> InItem) : Amount(InAmount), Name(InName), Image(InImage), Item(InItem)
{
}