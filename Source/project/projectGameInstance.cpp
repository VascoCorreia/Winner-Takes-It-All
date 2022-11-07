// Fill out your copyright notice in the Description page of Project Settings.


#include "projectGameInstance.h"
#include "Lobby/LobbyWidget.h"
#include "projectGameState.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

UprojectGameInstance::UprojectGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> LobbyBPClass(TEXT("/Game/MenuSystem/UI_Lobby_WBP"));
	if (!ensure(LobbyBPClass.Class != nullptr)) return;

	LobbyClass = LobbyBPClass.Class;

	Http = &FHttpModule::Get();

	/*UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	GameState = Cast<AprojectGameState>(World->GetGameState());

	UE_LOG(LogTemp, Warning, TEXT("GAME_INSTANCE: Init | Found GameState ? %i"), GameState != nullptr);*/
}

void UprojectGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *LobbyClass->GetName());
	/*UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	GameState = Cast<AprojectGameState>(World->GetGameState());

	UE_LOG(LogTemp, Warning, TEXT("GAME_INSTANCE: Init | Found GameState ? %i"), GameState != nullptr);*/
}

void UprojectGameInstance::SetupLevel()
{
	//UWorld* World = GetWorld();
	//if (!ensure(World != nullptr)) return;

	////AprojectGameState* GameState = Cast<AprojectGameState>(World->GetGameState());

	//AprojectGameState* GameState = Cast<AprojectGameState>(UGameplayStatics::GetGameState(World));

	//UE_LOG(LogTemp, Warning, TEXT("GAME_INSTANCE: SetupLevel (on Server) | Found GameState ? %i"), GameState != nullptr);
	//if (GameState != nullptr)
	//{
	//	GameState->SetupGameStateForLevel();
	//}
}

void UprojectGameInstance::Host()
{
	//HostAsListenServer();
	//HostAsDedicatedServer();
	CreateDedicatedServer();
}

/*
* Hosting a Listen Server (one of the Clients is also a server
*/
void  UprojectGameInstance::HostAsListenServer()
{
	// Host() can be called from the command line, and in that case, the Lobby would never be instantiated
	if (Lobby != nullptr)
	{
		Lobby->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/TopDownCPP/Maps/TopDownExampleMap.umap?listen");
}

/*
* Hosting a dedicated server - this is bad
*/
void UprojectGameInstance::HostAsDedicatedServer()
{
	// Host() can be called from the command line, and in that case, the Lobby would never be instantiated
	if (Lobby != nullptr)
	{
		Lobby->Teardown();
	}

	//UEngine* Engine = GetEngine();
	//if (!ensure(Engine != nullptr)) return;

	//Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FString PORT = FString(TEXT("7771"));
	FString Command = FString(TEXT("/Game/TopDownCPP/Maps/TopDownExampleMap.umap -server -log -PORT=7771"));
	PlayerController->ConsoleCommand(Command);
}

/*
* Interacts with a matchmaking server 
*/
void UprojectGameInstance::CreateDedicatedServer()
{
	// Call an API that will create the server
	if (!ensure(Http != nullptr)) return;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + "/api/matchmaking/start");
	SetRequestHeaders(Request); 
	Request->SetVerb("POST");
	Request->OnProcessRequestComplete().BindUObject(this, &UprojectGameInstance::OnCreateDedicatedServerComplete);
	Request->ProcessRequest();
}

void UprojectGameInstance::OnCreateDedicatedServerComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FResponse_CreateServer CreateServerResponse;
	GetStructFromJsonString<FResponse_CreateServer>(Response, CreateServerResponse);

	UE_LOG(LogTemp, Warning, TEXT("GAME_INSTANCE: Create_server_completed: %d"), CreateServerResponse.port);
	
	if (!ensure(Lobby != nullptr)) return;
	Lobby->AddToServerList(FString("127.0.0.1:") + FString::FromInt(CreateServerResponse.port));
}

void UprojectGameInstance::Join(const FString& Address)
{
	if (Lobby != nullptr)
	{
		Lobby->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	//UWorld* World = GetWorld();
	//if (!ensure(World != nullptr)) return;

	//AprojectGameState* GameState = Cast<AprojectGameState>(World->GetGameState());
	//if (!ensure(GameState != nullptr)) return;

	// TODO: Remove OnJoinServerCompleted from the header if you are not using it
	//OnJoinServerCompleted.ExecuteIfBound();

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	//GameState->SetupGameStateForLevel();
}

void UprojectGameInstance::LoadLobby()
{
	if (!ensure(LobbyClass != nullptr)) return;

	Lobby = CreateWidget<ULobbyWidget>(this, LobbyClass);
	if (!ensure(Lobby != nullptr)) return;

	Lobby->Setup();
}

void UprojectGameInstance::GoToLobby()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/UI_Lobby_WBP", ETravelType::TRAVEL_Absolute);
}

void UprojectGameInstance::SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request) {
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

bool UprojectGameInstance::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful) {
	if (!bWasSuccessful || !Response.IsValid()) return false;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) return true;
	else {
		UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
		return false;
	}
}

template <typename StructType>
void UprojectGameInstance::GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput) {
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutput, 0, 0);
}

template <typename StructType>
void UprojectGameInstance::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput) {
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}