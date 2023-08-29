#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "OD_BasePlayerState.generated.h"

class UOD_AbilitySystemComponent;
class UOD_CompInventory;

UCLASS()
class ODIN_API AOD_BasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
    AOD_BasePlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UOD_CompInventory* GetCompInventory();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Player|ASC")
		UOD_AbilitySystemComponent* AbilitySystemComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Player|Inventory")
		UOD_CompInventory* CompInventory = nullptr;

	virtual void BeginPlay() override;
};
