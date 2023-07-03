#include "Characters/OD_BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"

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

