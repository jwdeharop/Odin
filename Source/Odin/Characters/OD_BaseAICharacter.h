#pragma once

#include "Odin.h"
#include "Characters/OD_BaseTeamCharacter.h"
#include "OD_BaseAICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class ODIN_API AOD_BaseAICharacter : public AOD_BaseTeamCharacter
{
	GENERATED_BODY()

public:
	AOD_BaseAICharacter();

	UBehaviorTree* GetBehaviorTree() const;
protected:
	UPROPERTY(EditInstanceOnly, Category = "AI")
		UBehaviorTree* BehaviorTree = nullptr;

	virtual void BeginPlay() override;

};
