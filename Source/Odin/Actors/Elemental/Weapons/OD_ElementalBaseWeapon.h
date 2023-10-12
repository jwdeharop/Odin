#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OD_ElementalBaseWeapon.generated.h"

class AOD_ElementalProjectile;
enum class EOD_ElementalDamageType : uint8;
class USkeletalMeshComponent;

UCLASS(config=Game)
class AOD_ElementalBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
		void Server_ShootAtDirection(const FVector SocketLocation, const FRotator SocketRotator, const FVector& CameraLocation, const FVector& CameraVector);

	AOD_ElementalBaseWeapon();
	virtual void Shoot(EOD_ElementalDamageType DamageType, const FVector& CameraLocation, const FVector& CameraVector, const FVector& MuzzleLocation, const FRotator& MuzzleRotation);

	float GetRatio() const;
	void GetMuzzleInformation(FVector& SocketLocation, FRotator& SocketRotator) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Elemental | Weapons")
		USkeletalMeshComponent* SkeletalMesh = nullptr;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AOD_ElementalProjectile> ProjectileClass = nullptr;
	UPROPERTY(EditDefaultsOnly)
		float MaxWeaponDistanceToShoot = 1000.f;
	UPROPERTY(EditDefaultsOnly)
		float Ratio = 0.1f;
};

