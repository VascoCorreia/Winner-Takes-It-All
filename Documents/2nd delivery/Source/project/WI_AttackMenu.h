// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WI_AttackMenu.generated.h"


UCLASS()
class PROJECT_API UWI_AttackMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ACharacter* character = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* playerName = nullptr;

	FVector playerPosition;

	UFUNCTION(BlueprintCallable)
		void printToViewport();

	//Is this only running on the server if i do it like this?
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void clickedAttack(AActor* playerAttacking, AActor* playerBeingAttacked);
};
