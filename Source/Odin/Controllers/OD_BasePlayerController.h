#pragma once

#include "CoreMinimal.h"
#include "Actors/OD_BasePlayerState.h"
#include "GameFramework/PlayerController.h"
#include "OD_BasePlayerController.generated.h"


DECLARE_DELEGATE_OneParam(FOnControllerGetsPlayerState, AOD_BasePlayerState* /*PlayerState*/)

UCLASS()
class ODIN_API AOD_BasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnControllerGetsPlayerState OnControllerGetsPlayerState;

protected:
	virtual void OnRep_PlayerState() override;
	
};
