// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "../projectGameInstance.h"
#include "ServerRow.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "UObject/ConstructorHelpers.h"


ULobbyWidget::ULobbyWidget(const FObjectInitializer & ObjectInitializer) :Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/UI_ServerRow_WBP"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool ULobbyWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostBtn != nullptr)) return false;
	HostBtn->OnClicked.AddDynamic(this, &ULobbyWidget::HostServer);

	if (!ensure(JoinBtn != nullptr)) return false;
	JoinBtn->OnClicked.AddDynamic(this, &ULobbyWidget::OpenJoinMenu);

	if (!ensure(CancelJoinMenuBtn != nullptr)) return false;
	CancelJoinMenuBtn->OnClicked.AddDynamic(this, &ULobbyWidget::OpenMainMenu);

	if (!ensure(ConfirmJoinMenuBtn != nullptr)) return false;
	ConfirmJoinMenuBtn->OnClicked.AddDynamic(this, &ULobbyWidget::JoinServer);

	if (!ensure(QuitBtn != nullptr)) return false;
	QuitBtn->OnClicked.AddDynamic(this, &ULobbyWidget::QuitPressed);

	return true;
}

void ULobbyWidget::Setup()
{
	this->AddToViewport();
	// this might not work right away. In the Widget BP make sure that the top-level element in the tree has isFocusable ticked
	this->bIsFocusable = true;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void ULobbyWidget::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

void ULobbyWidget::AddToServerList(FString ServerAddress)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
	if (!ensure(Row != nullptr)) return;
	Row->SetText(ServerAddress);

	if (!ensure(ServerList != nullptr)) return;
	ServerList->AddChild(Row);
}

void ULobbyWidget::HostServer()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	
	UprojectGameInstance* GameInstance = Cast<UprojectGameInstance>(World->GetGameInstance());
	if (!ensure(GameInstance != nullptr)) return;

	GameInstance->Host();
}

void ULobbyWidget::JoinServer()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	UprojectGameInstance* GameInstance = Cast<UprojectGameInstance>(World->GetGameInstance());
	if (!ensure(GameInstance != nullptr)) return;

	if (!ensure(IPAddressField != nullptr)) return;

	const FString& Address = IPAddressField->GetText().ToString();
	GameInstance->Join(Address);
}

void ULobbyWidget::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void ULobbyWidget::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void ULobbyWidget::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}
