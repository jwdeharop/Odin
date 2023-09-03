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

	void Initialize();
	UOD_CompInventory* GetCompInventory();
	const UOD_CompInventory* GetCompInventory() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void OnRep_Owner() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Player|ASC")
		UOD_AbilitySystemComponent* AbilitySystemComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Player|Inventory")
		UOD_CompInventory* CompInventory = nullptr;

	virtual void BeginPlay() override;
	
};
