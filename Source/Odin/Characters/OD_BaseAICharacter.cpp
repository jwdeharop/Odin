#include "Characters/OD_BaseAICharacter.h"

AOD_BaseAICharacter::AOD_BaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

UBehaviorTree* AOD_BaseAICharacter::GetBehaviorTree() const
{
	return BehaviorTree;
}

void AOD_BaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
}
