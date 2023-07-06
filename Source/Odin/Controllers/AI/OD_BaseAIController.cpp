#include "Controllers/AI/OD_BaseAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/OD_BaseAICharacter.h"
#include "Perception/AIPerceptionComponent.h"

AOD_BaseAIController::AOD_BaseAIController() : Super()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Componnet"));
}

FGenericTeamId AOD_BaseAIController::GetGenericTeamId() const
{
	const AOD_BaseTeamCharacter* MyCharacter = Cast<AOD_BaseTeamCharacter>(GetPawn());
	return MyCharacter ? FGenericTeamId(static_cast<uint8>(MyCharacter->GetCurrentTeam())) : FGenericTeamId();
}

void AOD_BaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const AOD_BaseAICharacter* BaseCharacter = Cast<AOD_BaseAICharacter>(InPawn);
	UBehaviorTree* BehaviorTree = BaseCharacter ? BaseCharacter->GetBehaviorTree() : nullptr;
	if (!BehaviorTree)
		return;

	BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BehaviorTreeComponent->StartTree(*BehaviorTree);
	
}
