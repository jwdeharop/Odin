#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OD_ElementalPlayerController.generated.h"

UCLASS(config=Game)
class AOD_ElementalPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnPossess(APawn* InPawn) override;
	
};

