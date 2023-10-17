#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OD_BaseLibrary.generated.h"

class AOD_ElementalCharacter;
class AOD_ElementalPlayerState;
class AOD_ElementalPlayerController;

UCLASS()
class ODIN_API UOD_BaseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
		static AOD_ElementalPlayerController* GetLocalPlayerController(const UObject* WorldObjectContext);
	UFUNCTION(BlueprintPure)
		static AOD_ElementalPlayerState* GetLocalPlayerState(const UObject* WorldObjectContext);
	UFUNCTION(BlueprintPure)
		static AOD_ElementalCharacter* GetLocalPlayerCharacter(const UObject* WorldObjectContext);
};
