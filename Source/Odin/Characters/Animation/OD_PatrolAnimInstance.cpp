#include "Characters/Animation/OD_PatrolAnimInstance.h"
#include "Characters/OD_PatrolCharacter.h"

void UOD_PatrolAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// Only gather information if character is valid.
	if (!BaseCharacter.IsValid() || !CharacterMovement.IsValid())
		return;

	const TWeakObjectPtr<AOD_PatrolCharacter> PatrolCharacter = Cast<AOD_PatrolCharacter>(BaseCharacter.Get());
	if (!PatrolCharacter.IsValid())
		return;

	bIsUsingTorch = PatrolCharacter->IsUsingTorch();
}
