#pragma once

#include "CoreMinimal.h"
#include "Controllers/AI/OD_BaseAIController.h"
#include "OD_PatrolAIController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class ODIN_API AOD_PatrolAIController : public AOD_BaseAIController
{
	GENERATED_BODY()

public:
	AOD_PatrolAIController();

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
		UAISenseConfig_Sight* SightConfig = nullptr;
		
	
};
