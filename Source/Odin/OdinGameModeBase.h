// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OdinGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ODIN_API AOdinGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
