#include "Actors/Elemental/Weapons/OD_ElementalBaseWeapon.h"
#include "Actors/Elemental/Weapons/OD_ElementalProjectile.h"
#include "Libraries/OD_NetLibrary.h"

AOD_ElementalBaseWeapon::AOD_ElementalBaseWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;
}

void AOD_ElementalBaseWeapon::Shoot(EOD_ElementalDamageType DamageType)
{
	if (!ProjectileClass || !SkeletalMesh)
		return;

	const FVector SocketLocation = SkeletalMesh->GetSocketLocation("Muzzle");
	const FRotator SocketRotator = SkeletalMesh->GetSocketRotation("Muzzle");
	Server_ShootAtDirection(SocketLocation, SocketRotator);
}

void AOD_ElementalBaseWeapon::Server_ShootAtDirection_Implementation(const FVector SocketLocation, const FRotator SocketRotator)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetOwner<APawn>();
	GetWorld()->SpawnActor<AOD_ElementalProjectile>(ProjectileClass, SocketLocation, SocketRotator, SpawnParams);
}
