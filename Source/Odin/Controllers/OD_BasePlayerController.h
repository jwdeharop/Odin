#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "OD_BasePlayerController.generated.h"

UCLASS()
class ODIN_API AOD_BasePlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	virtual FGenericTeamId GetGenericTeamId() const override;
};
