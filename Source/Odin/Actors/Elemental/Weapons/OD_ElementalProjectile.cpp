#include "Actors/Elemental/Weapons/OD_ElementalProjectile.h"
#include "Characters/Elemental/OD_ElementalCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/OD_NetLibrary.h"

AOD_ElementalProjectile::AOD_ElementalProjectile()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	RootComponent = SphereComponent;
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void AOD_ElementalProjectile::SetCurrentDamage(EOD_ElementalDamageType DamageType)
{
	CurrentDamage = DamageType;
}

void AOD_ElementalProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const AActor* MyOwner = GetOwner();
	if (!UOD_NetLibrary::IsServer(this) || !MyOwner)
		return;

	// For now, we only want to interact with characters.
	AOD_ElementalCharacter* OtherCharacter = Cast<AOD_ElementalCharacter>(OtherActor);

	if (!OtherCharacter || MyOwner == OtherActor)
	{
		Destroy();
		return;
	}

	// If other character, we will apply damage to it.
	UGameplayStatics::ApplyPointDamage(OtherCharacter, OtherCharacter->CalculateDamageToMe(CurrentDamage), Hit.ImpactNormal, Hit, MyOwner->GetInstigatorController(), this, TSubclassOf <UDamageType>());
	Destroy();
}

void AOD_ElementalProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (UOD_NetLibrary::IsServer(this))
	{
		SphereComponent->OnComponentHit.AddUniqueDynamic(this, &AOD_ElementalProjectile::OnHit);
	}
}
