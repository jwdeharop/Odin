#include "Characters/Animation/OD_PatrolAnimInstance.h"
#include "Characters/OD_PatrolCharacter.h"

void UOD_PatrolAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (const AOD_PatrolCharacter* const PatrolCharacter = Cast<AOD_PatrolCharacter>(BaseCharacter.Get()))
	{
		bIsUsingTorch = PatrolCharacter->IsUsingTorch();
	}
}
