// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetText(FString Text);

protected:
	virtual bool Initialize() override;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* ServerAddress;

	UPROPERTY(Meta = (BindWidget))
	class UButton* JoinBtn;

private:
	
	UFUNCTION()
	void JoinServer();
};
