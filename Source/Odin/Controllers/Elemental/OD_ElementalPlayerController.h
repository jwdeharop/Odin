#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OD_ElementalPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnControllerGetsPlayerState, APlayerState*);

UCLASS(config=Game)
class AOD_ElementalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnControllerGetsPlayerState OnClientGetsPlayerState;

protected:
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;
	
};

