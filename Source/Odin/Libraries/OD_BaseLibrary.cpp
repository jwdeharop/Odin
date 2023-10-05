#include "Libraries/OD_BaseLibrary.h"
#include "Controllers/OD_BasePlayerController.h"

AOD_BasePlayerController* UOD_BaseLibrary::GetLocalPlayerController(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	return World ? World->GetFirstPlayerController<AOD_BasePlayerController>() : nullptr;
}

AOD_BasePlayerState* UOD_BaseLibrary::GetLocalPlayerState(const UObject* WorldContextObject)
{
	const AOD_BasePlayerController* PlayerController = GetLocalPlayerController(WorldContextObject);
	return PlayerController ? PlayerController->GetPlayerState<AOD_BasePlayerState>() : nullptr;
}
