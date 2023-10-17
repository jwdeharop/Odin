#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OD_GUILibrary.generated.h"

UCLASS()
class ODIN_API UOD_GUILibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
		static FLinearColor GetElementColor(const UObject* WorldObjectContext, EOD_ElementalDamageType ElementType);
	UFUNCTION(BlueprintPure)
		static TSoftObjectPtr<UTexture2D> GetElementIcon(const UObject* WorldObjectContext, EOD_ElementalDamageType ElementType);
};
