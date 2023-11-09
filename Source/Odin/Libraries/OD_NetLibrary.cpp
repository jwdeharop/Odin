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