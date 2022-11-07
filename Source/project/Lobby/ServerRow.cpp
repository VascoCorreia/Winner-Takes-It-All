// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "../projectGameInstance.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UServerRow::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(JoinBtn != nullptr)) return false;
	JoinBtn->OnClicked.AddDynamic(this, &UServerRow::JoinServer);

	return true;
}

void UServerRow::SetText(FString Text)
{
	if (!ensure(ServerAddress != nullptr)) return;
	ServerAddress->SetText(FText::AsCultureInvariant(Text));
}

void UServerRow::JoinServer()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	UprojectGameInstance* GameInstance = Cast<UprojectGameInstance>(World->GetGameInstance());
	if (!ensure(GameInstance != nullptr)) return;

	if (!ensure(ServerAddress != nullptr)) return;

	const FString& Address = ServerAddress->GetText().ToString();
	GameInstance->Join(Address);
}
