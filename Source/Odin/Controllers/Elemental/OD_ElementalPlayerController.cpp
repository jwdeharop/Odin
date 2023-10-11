#include "Controllers/Elemental/OD_ElementalPlayerController.h"
#include "Libraries/OD_NetLibrary.h"

void AOD_ElementalPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

float AOD_ElementalPlayerController::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
