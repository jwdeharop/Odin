#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "OD_BasePlayerState.generated.h"

class UOD_AbilitySystemComponent;

UCLASS()
class ODIN_API AOD_BasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
    AOD_BasePlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ASC")
		UOD_AbilitySystemComponent* AbilitySystemComponent = nullptr;
};
