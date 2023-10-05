#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OD_BaseLibrary.generated.h"

class AOD_BasePlayerController;
class AOD_BasePlayerState;

UCLASS()
class ODIN_API UOD_BaseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		static AOD_BasePlayerController* GetLocalPlayerController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure)
		static AOD_BasePlayerState* GetLocalPlayerState(const UObject* WorldContextObject);
};
