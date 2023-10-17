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
#include "UserWidget.h"
#include "Controllers/Elemental/OD_ElementalPlayerController.h"
#include "Interfaces/OD_InteractionInterface.h"
#include "Kismet/GameplayStatics.h"

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
		//CurrentWeapon->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::KeepWorldTransform, "GripPoint");
	}
}

void AOD_ElementalCharacter::Server_StopInteraction_Implementation(AActor* InteractingObject)
{
	IOD_InteractionInterface* InteractionInterface = Cast<IOD_InteractionInterface>(InteractingObject);
	if (!InteractionInterface)
		return;

	InteractionInterface->CancelInteraction();
}

void AOD_ElementalCharacter::Client_OnInteractionSuccess_Implementation()
{
	if (CompInteraction)
	{
		CompInteraction->OnInteractionSucess();
	}
}

void AOD_ElementalCharacter::OnInteractionSucess()
{
	Client_OnInteractionSuccess();
}

void AOD_ElementalCharacter::Server_StartInteraction_Implementation(AActor* InteractingObject)
{
	IOD_InteractionInterface* InteractionInterface = Cast<IOD_InteractionInterface>(InteractingObject);
	if (!InteractionInterface)
		return;

	InteractionInterface->OnInteractionSucess.BindUObject(this, &AOD_ElementalCharacter::OnInteractionSucess);
	InteractionInterface->StartInteraction(this);
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
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			CurrentWeapon = SpawnedWeapon;
			CurrentWeapon->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
		}

		OnRep_PlayerState();
	}

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (CompInteraction)
	{
		CompInteraction->InteractionAvailable.BindUObject(this, &AOD_ElementalCharacter::OnInteractionAvailable);
		CompInteraction->LostInteraction.AddUObject(this, &AOD_ElementalCharacter::OnInteractionLost);
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

		//Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AOD_ElementalCharacter::Shoot);
		EnhancedInputComponent->BindAction(StopShootAction, ETriggerEvent::Completed, this, &AOD_ElementalCharacter::StopShooting);

		// Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AOD_ElementalCharacter::StartInteraction);
		EnhancedInputComponent->BindAction(StopInteractionAction, ETriggerEvent::Completed, this, &AOD_ElementalCharacter::StopInteraction);

		EnhancedInputComponent->BindAction(ChangeDTAction, ETriggerEvent::Triggered, this, &AOD_ElementalCharacter::ChangeDT);
	}
}

float AOD_ElementalCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float TotalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	//REFACTOR NEEDED.
	AOD_ElementalPlayerState* MyPlayerState = GetPlayerState<AOD_ElementalPlayerState>();
	if (!MyPlayerState)
		return TotalDamage;

	MyPlayerState->LocalTakeDamage(Damage);
	
	return TotalDamage;
}

void AOD_ElementalCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurrentWeapon.Get())
	{
		CurrentWeapon->Destroy();
	}

	Super::EndPlay(EndPlayReason);
}

void AOD_ElementalCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	Client_PossessedBy(NewController);
}

void AOD_ElementalCharacter::OnClientStatsChanged(FOD_PlayerStats Fod_PlayerStats)
{
	if (!CurrentWeapon.Get())
		return;

	CurrentWeapon->BP_ChangeWeaponColor(Fod_PlayerStats.CurrentDamageType);
}

void AOD_ElementalCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AOD_ElementalPlayerState* MyPlayerState = GetPlayerState<AOD_ElementalPlayerState>())
	{
		MyPlayerState->OnClientStatsChanged.AddUObject(this, &AOD_ElementalCharacter::OnClientStatsChanged);
	}
}

void AOD_ElementalCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
	OutLocation = FirstPersonCameraComponent->GetComponentLocation();
	OutRotation = FirstPersonCameraComponent->GetComponentRotation();
}

void AOD_ElementalCharacter::NewServer_Shoot_Implementation(const FVector& SpawnLocation, const FRotator& SpawnRotator)
{
	const AOD_ElementalPlayerState* MyPlayerState = GetPlayerState<AOD_ElementalPlayerState>();
	if (!MyPlayerState)
		return;

	AOD_ElementalBaseWeapon* CurrentWeaponPtr = CurrentWeapon.Get();
	if (!CurrentWeaponPtr)
		return;

	CurrentWeaponPtr->NewShoot(MyPlayerState->GetCurrentDamageType(), SpawnLocation, SpawnRotator);
	Client_Shoot();
	GetWorldTimerManager().SetTimer(ShootingTimer, this, &AOD_ElementalCharacter::Shoot, CurrentWeapon->GetRatio());
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

EOD_ElementalDamageType AOD_ElementalCharacter::GetCurrentDamage() const
{
	const AOD_ElementalPlayerState* MyPlayerState = GetPlayerState<AOD_ElementalPlayerState>();
	if (!MyPlayerState)
		return EOD_ElementalDamageType::Synthetic;

	return MyPlayerState->GetCurrentDamageType();
}

void AOD_ElementalCharacter::Server_StopShoot_Implementation()
{
	if (ShootingTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(ShootingTimer);
	}
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

void AOD_ElementalCharacter::Client_PossessedBy_Implementation(AController* NewController)
{
	if (AOD_ElementalPlayerController* MyController = Cast<AOD_ElementalPlayerController>(NewController))
	{
		MyHud = CreateWidget(MyController, HUD, TEXT("HUD"));
		if (MyHud)
		{
			MyHud->AddToViewport();
		}
	}

	OnClientPossessed.Broadcast(NewController);
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
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!CurrentWeapon.Get() || !PlayerController)
		return;

	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = CurrentWeapon->GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 0.0f, 10.0f));
	NewServer_Shoot(SpawnLocation, SpawnRotation);
}

void AOD_ElementalCharacter::StopShooting()
{
	Server_StopShoot();
}

void AOD_ElementalCharacter::StartInteraction()
{
	if (CompInteraction)
	{
		Server_StartInteraction(CompInteraction->GetCurrentInteractActor());
		CompInteraction->StartInteraction();
	}
}

void AOD_ElementalCharacter::StopInteraction()
{
	if (CompInteraction)
	{
		Server_StopInteraction(CompInteraction->GetCurrentInteractActor());
		CompInteraction->StopInteraction();
	}
}

void AOD_ElementalCharacter::ChangeDT()
{
	AOD_ElementalPlayerState* MyPlayerState = GetPlayerState<AOD_ElementalPlayerState>();
	if (!MyPlayerState)
		return;

	MyPlayerState->Server_ChangePrimaryDamageType();
}

void AOD_ElementalCharacter::OnInteractionAvailable()
{
	BP_ChangeInteractionWidgetInformation(true);
}

void AOD_ElementalCharacter::OnInteractionLost()
{
	BP_ChangeInteractionWidgetInformation(false);
}

void AOD_ElementalCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOD_ElementalCharacter, CurrentWeapon);
}
