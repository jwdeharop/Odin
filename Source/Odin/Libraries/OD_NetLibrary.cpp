#include "Libraries/OD_NetLibrary.h"

bool UOD_NetLibrary::IsServer(const UObject* WorldContextObject)
{
	// Checking NetMode < NM_Client is always some variety of server.
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World && World->GetNetMode() < NM_Client;
}

bool UOD_NetLibrary::IsClient(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World && World->GetNetMode() != ENetMode::NM_DedicatedServer;
}

bool UOD_NetLibrary::IsDedicatedServer(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World && World->GetNetMode() == NM_DedicatedServer;
}

bool UOD_NetLibrary::IsSimulatedClient(const APawn* Pawn)
{
	return Pawn && Pawn->GetLocalRole() == ENetRole::ROLE_SimulatedProxy;
}

APlayerController* UOD_NetLibrary::GetMainController(const UObject* WorldContextObject)
{
	// Get world context (containing player controllers)
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// Loop on player controllers
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			// Get player controller from iterator
			APlayerController* PlayerController = Iterator->Get();

			// Get local player if exist
			ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

			// If it's local and id is 0, it's the main controller
			if (LocalPlayer != nullptr && LocalPlayer->GetControllerId() == 0)
			{
				return PlayerController;
			}
		}
	}

	// Not found
	return nullptr;
}