#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Characters/OD_BaseTeamCharacter.h"
#include "OD_BaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
class UOD_CompInteraction;

UCLASS()
class ODIN_API AOD_BaseCharacter : public AOD_BaseTeamCharacter
{
	GENERATED_BODY()

public:
	AOD_BaseCharacter();
	bool IsHoldingItem() const { return bIsHoldingItem; }

private:
	float MaxWalkSpeed = 0.f;
	float MaxCrouchSpeed = 0.f;
	bool bIsHoldingItem = false;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Input")
		UInputAction* InputActionCrouch = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Input")
		UInputAction* InputActionJump = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Interact")
		UOD_CompInteraction* CompInteraction = nullptr;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void BaseCrouch(const FInputActionValue& InputActionValue);

};
