#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "OD_BaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class ODIN_API AOD_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AOD_BaseCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USpringArmComponent* CameraBoom = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UCameraComponent* FollowCamera = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Input")
		UInputMappingContext* InputMapping = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Input")
		UInputAction* InputActionMove = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Input")
		UInputAction* InputActionLook = nullptr;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);

};
