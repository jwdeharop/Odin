#include "Characters/OD_BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

namespace AOD_BaseCharacter_Consts
{
	constexpr float ConstMaxCrouchSpeed = 300.f;
	constexpr float ConstMaxSpeed = 600.f;
}

AOD_BaseCharacter::AOD_BaseCharacter() : Super()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom);
}

void AOD_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = PlayerController ? ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()) : nullptr;
	if (!EnhancedInputSubsystem)
		return;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
		return;

	EnhancedInputSubsystem->ClearAllMappings();
	EnhancedInputSubsystem->AddMappingContext(InputMapping, 0);

	EnhancedInputComponent->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &AOD_BaseCharacter::Move);
	EnhancedInputComponent->BindAction(InputActionLook, ETriggerEvent::Triggered, this, &AOD_BaseCharacter::Look);
	EnhancedInputComponent->BindAction(InputActionCrouch, ETriggerEvent::Completed, this, &AOD_BaseCharacter::BaseCrouch);
}

void AOD_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		MaxWalkSpeed = MovementComponent->GetMaxSpeed();
		MaxCrouchSpeed = MovementComponent->MaxWalkSpeedCrouched;
	}
	else
	{
		MaxWalkSpeed = AOD_BaseCharacter_Consts::ConstMaxSpeed;
		MaxCrouchSpeed = AOD_BaseCharacter_Consts::ConstMaxCrouchSpeed;
	}
}

void AOD_BaseCharacter::Move(const FInputActionValue& InputActionValue)
{
	if (!Controller)
		return;
	
	const FVector2D& MoveValue = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

	if (MoveValue.Y != 0.f)
	{
		// Get forward vector.
		const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(Direction, MoveValue.Y);
	}

	if (MoveValue.X != 0.f)
	{
		// Get right vector.
		const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(Direction, MoveValue.X);
	}
}

void AOD_BaseCharacter::Look(const FInputActionValue& InputActionValue)
{
	if (!Controller)
		return;
	
	const FVector2D& LookValue = InputActionValue.Get<FVector2D>();

	if (LookValue.X != 0.f)
	{
		AddControllerYawInput(LookValue.X);
	}

	if (LookValue.Y != 0.f)
	{
		AddControllerPitchInput(LookValue.Y);
	}
}

void AOD_BaseCharacter::BaseCrouch(const FInputActionValue& InputActionValue)
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bWantsToCrouch = !MovementComponent->bWantsToCrouch;

	if (MovementComponent->bWantsToCrouch)
	{
		Crouch(true);
	}
	else
	{
		UnCrouch(false);
	}
}
