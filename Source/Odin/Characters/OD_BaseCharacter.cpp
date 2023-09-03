#include "Characters/OD_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/OD_CompInteraction.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/OD_BasePlayerState.h"
#include "Components/OD_AbilitySystemComponent.h"
#include "Components/OD_CompInventory.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/OD_InteractionInterface.h"
#include "Libraries/OD_NetLibrary.h"

namespace AOD_BaseCharacter_Consts
{
	constexpr float ConstMaxCrouchSpeed = 300.f;
	constexpr float ConstMaxSpeed = 600.f;
}

AOD_BaseCharacter::AOD_BaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	CompInteraction = CreateDefaultSubobject<UOD_CompInteraction>(TEXT("Comp Interaction"));
	CameraBoom->SetupAttachment(RootComponent);
	FollowCamera->SetupAttachment(CameraBoom);
}

void AOD_BaseCharacter::StartInteraction()
{
	if (CompInteraction)
	{
		CompInteraction->StartInteraction();
	}
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
	EnhancedInputComponent->BindAction(InputActionJump, ETriggerEvent::Started, this, &AOD_BaseCharacter::Jump);
	EnhancedInputComponent->BindAction(InputActionInteract, ETriggerEvent::Completed, this, &AOD_BaseCharacter::StartInteraction);
	EnhancedInputComponent->BindAction(InputActionChangeInventoryType, ETriggerEvent::Completed, this, &AOD_BaseCharacter::ChangeInventoryType);
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

void AOD_BaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// CompInteract will only exist on the client sides.
	if (!UOD_NetLibrary::IsDedicatedServer(this))
		return;

	if (!IsLocallyControlled())
	{
		CompInteraction->DestroyComponent();
		CompInteraction = nullptr;
	}
}

void AOD_BaseCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
	OutRotation = FollowCamera->GetComponentRotation();
}

void AOD_BaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystemComponent();
}

void AOD_BaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilitySystemComponent();
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

void AOD_BaseCharacter::ChangeInventoryType(const FInputActionValue& InputActionValue)
{
	UOD_CompInventory* CompInventory = GetCompInventory();
	if (!CompInventory)
		return;

	constexpr bool bIncreaseInventoryType = true;
	CompInventory->Server_ChangeInventoryType(bIncreaseInventoryType);
}

void AOD_BaseCharacter::InitAbilitySystemComponent()
{
	AOD_BasePlayerState* MyPlayerState = GetPlayerState<AOD_BasePlayerState>();
	if (!MyPlayerState)
		return;

	AbilitySystemComponent = Cast<UOD_AbilitySystemComponent>(MyPlayerState->GetAbilitySystemComponent());
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitAbilityActorInfo(MyPlayerState, this);
	}
}

UOD_CompInventory* AOD_BaseCharacter::GetCompInventory()
{
	AOD_BasePlayerState* ODPlayerState = GetPlayerState<AOD_BasePlayerState>();
	return ODPlayerState ? ODPlayerState->GetCompInventory() : nullptr;
}