#pragma once

#include "Odin.h"
#include "GameFramework/Character.h"
#include "OD_BaseTeamCharacter.generated.h"

UCLASS()
class ODIN_API AOD_BaseTeamCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	EOD_SelfDistinction GetCurrentTeam() const;

protected:
	UPROPERTY(EditAnywhere, Category = "Team")
		EOD_SelfDistinction CurrentTeam = EOD_SelfDistinction::None;

};
