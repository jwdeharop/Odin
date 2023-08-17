#include "OD_AssetManager.h"
#include "AbilitySystemGlobals.h"
#include "StructuredLog.h"

UOD_AssetManager& UOD_AssetManager::Get() 
{
	UOD_AssetManager* Singleton = Cast<UOD_AssetManager>(GEngine->AssetManager);

	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOGFMT(LogTemp, Fatal, "Invalid AssetManager in DefaultEngine.ini, must be UOD_AssetManager!");
	return *NewObject<UOD_AssetManager>(); // never calls this
}

void UOD_AssetManager::StartInitialLoading() 
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}