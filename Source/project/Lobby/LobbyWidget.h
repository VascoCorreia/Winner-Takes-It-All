// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ULobbyWidget(const FObjectInitializer& ObjectInitializer);

	void Setup();

	void Teardown();

	void AddToServerList(FString ServerAddress);

protected:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* HostBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinBtn = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* QuitBtn = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UButton* CancelJoinMenuBtn;

	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmJoinMenuBtn;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IPAddressField;

private:
	TSubclassOf<class UUserWidget> ServerRowClass;

	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void OpenMainMenu();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void QuitPressed();

};
