#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "OD_BaseAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;


// All Controllers that wants to use a Behavior Tree should inherit from this class.
UCLASS()
class ODIN_API AOD_BaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	AOD_BaseAIController();

protected:
	UPROPERTY()
		UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;
	UPROPERTY()
		UBlackboardComponent* BlackboardComponent = nullptr;

	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void OnPossess(APawn* InPawn) override;
};