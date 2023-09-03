#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OD_NetLibrary.generated.h"

UCLASS()
class ODIN_API UOD_NetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Odin Net Library", meta = (WorldContext = "WorldContextObject"))
		static bool IsServer(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "Odin Net Library", meta = (WorldContext = "WorldContextObject"))
		static bool IsClient(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "Odin Net Library", meta = (WorldContext = "WorldContextObject"))
		static bool IsDedicatedServer(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "Odin Net Library", meta = (WorldContext = "WorldContextObject"))
		static bool IsSimulatedClient(const APawn* Pawn);
	static APlayerController* GetMainController(const UObject* WorldContextObject);
};
