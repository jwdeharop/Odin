#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "OD_AssetManager.generated.h"

UCLASS()
class ODIN_API UOD_AssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UOD_AssetManager& Get();

	/** Starts initial load, gets called from InitializeObjectReferences */
	virtual void StartInitialLoading() override;
	
};
