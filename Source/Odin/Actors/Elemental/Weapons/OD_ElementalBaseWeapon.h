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
		void Server_ShootAtDirection(const FVector SocketLocation, const FRotator SocketRotator);

	AOD_ElementalBaseWeapon();
	virtual void Shoot(EOD_ElementalDamageType DamageType);

	float GetRatio() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Elemental | Weapons")
		USkeletalMeshComponent* SkeletalMesh = nullptr;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AOD_ElementalProjectile> ProjectileClass = nullptr;

private:
	UPROPERTY(EditDefaultsOnly)
		float Ratio = 0.1f;
};

