#pragma once

#include "CoreMinimal.h"
#include "Odin.h"
#include "GameFramework/Actor.h"
#include "OD_ElementalProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS(config=Game)
class AOD_ElementalProjectile : public AActor
{
	GENERATED_BODY()

public:
	AOD_ElementalProjectile();

private:
	UPROPERTY(EditDefaultsOnly)
		USphereComponent* SphereComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* StaticMeshComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
		UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	EOD_ElementalDamageType CurrentDamage = EOD_ElementalDamageType::Basic;

protected:
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;
};

