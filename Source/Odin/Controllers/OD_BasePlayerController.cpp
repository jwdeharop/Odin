#include "Controllers/OD_BasePlayerController.h"
#include "Characters/OD_BaseTeamCharacter.h"

FGenericTeamId AOD_BasePlayerController::GetGenericTeamId() const
{
	const AOD_BaseTeamCharacter* MyCharacter = Cast<AOD_BaseTeamCharacter>(GetPawn());
	return MyCharacter ? FGenericTeamId(static_cast<uint8>(MyCharacter->GetCurrentTeam())) : FGenericTeamId();
}

