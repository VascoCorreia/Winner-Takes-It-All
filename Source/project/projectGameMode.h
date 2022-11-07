// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "projectGameMode.generated.h"



UCLASS(minimalapi)
class AprojectGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AprojectGameMode();

	virtual void PostLogin(class AprojectPlayerController* newPlayer);

	/*template<class AprojectGamestate>
	AprojectGamestate* GetGameState() const;*/
};



