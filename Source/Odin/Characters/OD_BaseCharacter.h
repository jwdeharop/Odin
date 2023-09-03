#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "OD_BaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
class UOD_AbilitySystemComponent;
class UOD_CompInteraction;
class UOD_CompInventory;

UCLASS()
class ODIN_API AOD_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AOD_BaseCharacter(const FObjectInitializer& ObjectInitializer);

	void StartInteraction();

	bool IsHoldingItem() const { return bIsHoldingItem; }
	UOD_CompInventory* GetCompInventory();

private:
	float MaxWalkSpeed = 0.f;
	float MaxCrouchSpeed = 0.f;
	bool bIsCrouching = false;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Input")
		UInputAction* InputActionInteract = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Input")
		UInputAction* InputActionChangeInventoryType = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Interact")
		UOD_CompInteraction* CompInteraction = nullptr;

	TWeakObjectPtr<UOD_AbilitySystemComponent> AbilitySystemComponent = nullptr;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void BaseCrouch(const FInputActionValue& InputActionValue);
	void ChangeInventoryType(const FInputActionValue& InputActionValue);

	void InitAbilitySystemComponent();
};
