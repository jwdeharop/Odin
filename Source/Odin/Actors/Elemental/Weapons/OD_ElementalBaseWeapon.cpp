#include "Actors/Elemental/Weapons/OD_ElementalBaseWeapon.h"
#include "Actors/Elemental/Weapons/OD_ElementalProjectile.h"

AOD_ElementalBaseWeapon::AOD_ElementalBaseWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;
}

void AOD_ElementalBaseWeapon::Shoot(EOD_ElementalDamageType DamageType)
{
	if (!ProjectileClass || !SkeletalMesh)
		return;

	FVector SocketLocation;
	FRotator SocketRotator;
	GetMuzzleInformation(SocketLocation, SocketRotator);
	Server_ShootAtDirection(SocketLocation, SocketRotator);
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

void AOD_ElementalBaseWeapon::Server_ShootAtDirection_Implementation(const FVector SocketLocation, const FRotator SocketRotator)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetOwner<APawn>();
	GetWorld()->SpawnActor<AOD_ElementalProjectile>(ProjectileClass, SocketLocation, SocketRotator, SpawnParams);
}
