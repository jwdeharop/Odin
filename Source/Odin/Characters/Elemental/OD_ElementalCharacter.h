#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Odin.h"
#include "OD_ElementalCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UInputMappingContext;
class UInputAction;
class AOD_ElementalBaseWeapon;
class UAnimMontage;
class UOD_CompDamage;
class UOD_CompInteraction;

UCLASS(config=Game)
class AOD_ElementalCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
		USkeletalMeshComponent* Mesh1P = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
		UInputAction* JumpAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
		UInputAction* MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* LookAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ShootAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* StopShootAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* InteractAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* StopInteractionAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Elemental | Weapon")
		TSubclassOf<AOD_ElementalBaseWeapon> CurrentWeaponClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ShootMontage = nullptr;
	UPROPERTY(EditDefaultsOnly)
		UOD_CompDamage* CompDamage = nullptr;
	UPROPERTY(ReplicatedUsing=OnRep_CurrentWeapon)
		TWeakObjectPtr<AOD_ElementalBaseWeapon> CurrentWeapon = nullptr;

	UFUNCTION()
		void OnRep_CurrentWeapon();

public:
	FTimerHandle ShootingTimer;

	AOD_ElementalCharacter();
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	float CalculateDamageToMe(EOD_ElementalDamageType DamageType) const;
	EOD_ElementalDamageType GetCurrentDamage() const;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	UOD_CompInteraction* GetCompInteraction() { return CompInteraction; }
protected:
	UPROPERTY(EditDefaultsOnly)
		UOD_CompInteraction* CompInteraction = nullptr;

	UFUNCTION(Server, Reliable)
		void Server_Shoot(const FVector& CameraLocation, const FVector& CameraDirection);
	UFUNCTION(Server, Reliable)
		void Server_StopShoot();
	UFUNCTION(Client, Reliable)
		void Client_Shoot();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_ChangeInteractionWidgetInformation(bool bIsAvailable);

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Shoot();
	void StopShooting();
	void StartInteraction();
	void StopInteraction();
	void OnInteractionAvailable();

	void OnInteractionLost();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};

