#pragma once

#include "Odin.h"
#include "GameFramework/Actor.h"
#include "OD_BaseItem.generated.h"

UCLASS()
class ODIN_API AOD_BaseItem : public AActor
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly)
        FName ItemName = NAME_None;
	UPROPERTY(EditDefaultsOnly)
		EOD_InventoryType InventoryType = EOD_InventoryType::None;
};
