#include "Tasks/BTTask_FillSplinePoints.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/OD_PatrolCharacter.h"

UBTTask_FillSplinePoints::UBTTask_FillSplinePoints() : Super()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_FillSplinePoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (GetSelectedBlackboardKey().IsNone())
		return EBTNodeResult::Failed;

	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if (!MyBlackboard)
		return EBTNodeResult::Failed;

	const AAIController* MyController = OwnerComp.GetAIOwner();
	const AOD_PatrolCharacter* PatrolCharacter = MyController ? MyController->GetPawn<AOD_PatrolCharacter>() : nullptr;

	if (!PatrolCharacter)
		return EBTNodeResult::Failed;

	const TArray<FVector>& SplinePoints = PatrolCharacter->GetSplinePoints();
	MyBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), SplinePoints[CurrentSplineIndex]);
	if (CurrentSplineIndex < SplinePoints.Num() - 1)
	{
		++CurrentSplineIndex;
		return EBTNodeResult::Succeeded;
	}

	CurrentSplineIndex = 0;
	return EBTNodeResult::Succeeded;
}
