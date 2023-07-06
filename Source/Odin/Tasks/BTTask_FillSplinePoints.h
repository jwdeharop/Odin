#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FillSplinePoints.generated.h"

UCLASS()
class ODIN_API UBTTask_FillSplinePoints : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FillSplinePoints();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	int32 CurrentSplineIndex = 0;
	
};
