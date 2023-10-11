#include "OD_ElementalCharacter.h"
#include "Actors/Elemental/Weapons/OD_ElementalBaseWeapon.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Elemental/OD_CompDamage.h"
#include "Components/OD_CompInteraction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Libraries/OD_NetLibrary.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"
#include "UnrealNetwork.h"

namespace AOD_ElementalCharacter_Consts
{
	constexpr float ResistanceDamageMultiplier = 0.5f;
	constexpr float NemesisDamageMultiplier = 0.25f;
	constexpr float FullDamageMultiplier = 1.f;
	constexpr float PreyDamageMultiplier = 2.f;
	constexpr float BaseDamage = 10.f;
}

void AOD_ElementalCharacter::OnRep_CurrentWeapon()
{
	if (CurrentWeapon.Get())
	{
		CurrentWeapon->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::KeepWorldTransform, "GripPoint");
	}
}

AOD_ElementalCharacter::AOD_ElementalCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GetMesh()->SetOwnerNoSee(true);

	CompDamage = CreateDefaultSubobject<UOD_CompDamage>(TEXT("CompDamage"));
	CompInteraction = CreateDefaultSubobject<UOD_CompInteraction>("CompInteraction");
}

void AOD_ElementalCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UOD_NetLibrary::IsDedicatedServer(this))
	{
		CompInteraction->DestroyComponent();
		CompInteraction = nullptr;
	}

	if (UOD_NetLibrary::IsServer(this))
	{
		const FVector SocketLocation = GetMesh1P()->GetSocketLocation("GripPoint");
		const FRotator SocketRotator = GetMesh1P()->GetSocketRotation("GripPoint");
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		if (AOD_ElementalBaseWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AOD_ElementalBaseWeapon>(CurrentWeaponClass, SocketLocation, SocketRotator, SpawnParams))
		{
			CurrentWeapon = SpawnedWeapon;
			OnRep_CurrentWeapon();
		}
	}

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AOD_ElementalCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOD_ElementalCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOD_ElementalCharacter::Look);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AOD_ElementalCharacter::Shoot);
		EnhancedInputComponent->BindAction(StopShootAction, ETriggerEvent::Completed, this, &AOD_ElementalCharacter::StopShooting);
	}
}

float AOD_ElementalCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void AOD_ElementalCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
	const AOD_ElementalBaseWeapon* CurrentWeaponPtr = CurrentWeapon.Get(); 
	if (!CurrentWeaponPtr)
		return;

	FVector SocketLocation;
	FRotator SocketRotator;
	CurrentWeaponPtr->GetMuzzleInformation(SocketLocation, SocketRotator);

	OutLocation = SocketLocation;
	OutRotation = SocketRotator;
}

float AOD_ElementalCharacter::CalculateDamageToMe(EOD_ElementalDamageType DamageType) const
{
	if (!CompDamage)
		return AOD_ElementalCharacter_Consts::BaseDamage;
	
	const AOD_ElementalPlayerState* MyPlayerState = GetPlayerState<AOD_ElementalPlayerState>();
	if (!MyPlayerState)
		return CompDamage->GetBaseDamage();

	return CompDamage->GetDamage(MyPlayerState->GetCurrentDamageType(), DamageType);
}

void AOD_ElementalCharacter::Server_StopShoot_Implementation()
{
	if (ShootingTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(ShootingTimer);
	}
}

void AOD_ElementalCharacter::Server_Shoot_Implementation()
{
	const AOD_ElementalPlayerState* MyPlayerState = GetPlayerState<AOD_ElementalPlayerState>();
	if (!MyPlayerState)
		return;

	AOD_ElementalBaseWeapon* CurrentWeaponPtr = CurrentWeapon.Get();
	if (!CurrentWeaponPtr)
		return;

	CurrentWeaponPtr->Shoot(MyPlayerState->GetCurrentDamageType());

	const float WeaponRatio = CurrentWeaponPtr->GetRatio();
	if (WeaponRatio > 0.f)
	{
		GetWorldTimerManager().SetTimer(ShootingTimer, this, &AOD_ElementalCharacter::Shoot, WeaponRatio);
	}

	Client_Shoot();
}

void AOD_ElementalCharacter::Client_Shoot_Implementation()
{
	if (IsLocallyControlled())
	{
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (!AnimInstance)
			return;

		AnimInstance->Montage_Play(ShootMontage);
	}
}

void AOD_ElementalCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AOD_ElementalCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AOD_ElementalCharacter::Shoot()
{
	Server_Shoot();
}

void AOD_ElementalCharacter::StopShooting()
{
	Server_StopShoot();
}

void AOD_ElementalCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOD_ElementalCharacter, CurrentWeapon);
}
