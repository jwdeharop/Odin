#include "Actors/Elemental/Weapons/OD_ElementalBaseWeapon.h"
#include "Actors/Elemental/Weapons/OD_ElementalProjectile.h"
#include "Characters/Elemental/OD_ElementalCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

void AOD_ElementalBaseWeapon::Server_ShootAtDirection_Implementation(const FVector SocketLocation, const FRotator SocketRotator, const FVector& CameraLocation, const FVector& CameraVector)
{
	const AOD_ElementalCharacter* OwnerCharacter = Cast<AOD_ElementalCharacter>(GetOwner());
	if (!OwnerCharacter)
		return;

	const FVector LocationToSendProjectile = CameraLocation + CameraVector * 1000.f;
	FVector BulletVelocity = LocationToSendProjectile - SocketLocation;
	BulletVelocity.Normalize();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetOwner<APawn>();
	if (AOD_ElementalProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AOD_ElementalProjectile>(ProjectileClass, SocketLocation, SocketRotator, SpawnParams))
	{
		SpawnedProjectile->ProjectileMovementComponent->Velocity = BulletVelocity * SpawnedProjectile->ProjectileMovementComponent->GetMaxSpeed();
		SpawnedProjectile->SetCurrentDamage(OwnerCharacter->GetCurrentDamage());
	}
}

AOD_ElementalBaseWeapon::AOD_ElementalBaseWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;
}

void AOD_ElementalBaseWeapon::Shoot(EOD_ElementalDamageType DamageType, const FVector& CameraLocation, const FVector& CameraVector, const FVector& MuzzleLocation, const FRotator& MuzzleRotation)
{
	if (!ProjectileClass || !SkeletalMesh)
		return;

	Server_ShootAtDirection(MuzzleLocation, MuzzleRotation, CameraLocation, CameraVector);
}

void AOD_ElementalBaseWeapon::NewShoot(EOD_ElementalDamageType DamageType, const FVector& SpawnLocation, const FRotator& SpawnRotator)
{
	const AOD_ElementalCharacter* OwnerCharacter = Cast<AOD_ElementalCharacter>(GetOwner());
	if (!OwnerCharacter)
		return;

	if (!ProjectileClass || !SkeletalMesh)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetOwner<APawn>();
	if (AOD_ElementalProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AOD_ElementalProjectile>(ProjectileClass, SpawnLocation, SpawnRotator, SpawnParams))
	{
		SpawnedProjectile->SetCurrentDamage(OwnerCharacter->GetCurrentDamage());
	}
}

float AOD_ElementalBaseWeapon::GetRatio() const
{
	return Ratio;
}

void AOD_ElementalBaseWeapon::GetMuzzleInformation(FVector& SocketLocation, FRotator& SocketRotator) const
{
	SocketLocation = SkeletalMesh->GetSocketLocation("Muzzle");
	SocketRotator = SkeletalMesh->GetSocketRotation("Muzzle");
}

void AOD_ElementalBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	const AOD_ElementalCharacter* OwnerCharacter = Cast<AOD_ElementalCharacter>(GetOwner());
	if (!OwnerCharacter || !OwnerCharacter->IsLocallyControlled())
		return;

	BP_ChangeWeaponColor(EOD_ElementalDamageType::Synthetic);
}
