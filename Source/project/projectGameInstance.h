// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "projectGameInstance.generated.h"

USTRUCT()
struct FRequest_JoinServer {
	GENERATED_BODY()
	UPROPERTY() int32 port;

	FRequest_JoinServer() {}
};

USTRUCT()
struct FResponse_CreateServer {
	GENERATED_BODY()
	UPROPERTY() int32 port;

	FResponse_CreateServer() {}
};

DECLARE_DELEGATE(FJoinServerCompletedDelegate)
UCLASS()
class PROJECT_API UprojectGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UprojectGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION()
	void SetupLevel();

	// Exec attribute makes this function executable from the in-game console (invoked with tilde ~)
	UFUNCTION(Exec)
		void Host();

	UFUNCTION(Exec)
		void Join(const FString& Address);

	UFUNCTION(BlueprintCallable)
		void LoadLobby();

	virtual void GoToLobby();

	FJoinServerCompletedDelegate OnJoinServerCompleted;

private:
	//class AprojectGameState* GameState;

	TSubclassOf<class UUserWidget> LobbyClass;
	class ULobbyWidget* Lobby;

	FHttpModule* Http;
	FString ApiBaseUrl = "http://127.0.0.1:3000";

	void HostAsListenServer();

	void HostAsDedicatedServer();

	void CreateDedicatedServer();

	void OnCreateDedicatedServerComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request);
	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

	template <typename StructType>
	void GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput);
	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput);

};
