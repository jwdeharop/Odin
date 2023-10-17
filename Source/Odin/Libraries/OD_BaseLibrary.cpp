#include "Libraries/OD_BaseLibrary.h"

#include "Controllers/Elemental/OD_ElementalPlayerController.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"
#include "Characters/Elemental/OD_ElementalCharacter.h"

AOD_ElementalPlayerController* UOD_BaseLibrary::GetLocalPlayerController(const UObject* WorldObjectContext)
{
	ensureAlways(WorldObjectContext);
	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldObjectContext);
	return World ? World->GetFirstPlayerController<AOD_ElementalPlayerController>() : nullptr;
}

AOD_ElementalPlayerState* UOD_BaseLibrary::GetLocalPlayerState(const UObject* WorldObjectContext)
{
	const AOD_ElementalPlayerController* LocalPlayerController = GetLocalPlayerController(WorldObjectContext);
	return LocalPlayerController ? LocalPlayerController->GetPlayerState<AOD_ElementalPlayerState>() : nullptr;
}

AOD_ElementalCharacter* UOD_BaseLibrary::GetLocalPlayerCharacter(const UObject* WorldObjectContext)
{
	const AOD_ElementalPlayerController* LocalPlayerController = GetLocalPlayerController(WorldObjectContext);
	return LocalPlayerController ? LocalPlayerController->GetPawn<AOD_ElementalCharacter>() : nullptr;
}