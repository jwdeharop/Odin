#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OD_BaseCharacter.generated.h"

UCLASS()
class ODIN_API AOD_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
