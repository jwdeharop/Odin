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

void AOD_ElementalBaseWeapon::Shoot(EOD_ElementalDamageType DamageType, const FVector& CameraLocation, const FVector& CameraVector)
{
	if (!ProjectileClass || !SkeletalMesh)
		return;

	FVector SocketLocation;
	FRotator SocketRotator;
	GetMuzzleInformation(SocketLocation, SocketRotator);
	Server_ShootAtDirection(SocketLocation, SocketRotator, CameraLocation, CameraVector);
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