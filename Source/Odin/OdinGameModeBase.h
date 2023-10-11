#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameLiftServerSDK.h"
#include "OdinGameModeBase.generated.h"

UCLASS()
class ODIN_API AOdinGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOdinGameModeBase();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
private:
	FProcessParameters Params;

	void InitGameLift();
	
};
