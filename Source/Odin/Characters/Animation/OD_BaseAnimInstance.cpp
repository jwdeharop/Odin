#include "Characters/Animation/OD_BaseAnimInstance.h"
#include "Characters/OD_BaseCharacter.h"
#include "Characters/OD_BaseTeamCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

namespace UOD_BaseAnimInstance_Consts
{
	constexpr float VelocityGroundThreshold = 3.f;
}

void UOD_BaseAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	BaseCharacter = Cast<AOD_BaseTeamCharacter>(GetOwningActor());
	CharacterMovement = BaseCharacter.IsValid() ? BaseCharacter->GetMovementComponent() : nullptr;
}

void UOD_BaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// Only gather information if character is valid.
	if (!BaseCharacter.IsValid() || !CharacterMovement.IsValid())
		return;

	// Get Ground velocity from the character velocity.
	Velocity = CharacterMovement->Velocity;
	GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);

	//Set Should Move to true only if ground speed is above a small threshold
	//(to prevent incredibly small velocities from triggering animations) and if
	//there is currently acceleration (input) applied.
	const bool bHasVelocity = UKismetMathLibrary::NotEqualExactly_VectorVector(Velocity, FVector::ZeroVector);
	const bool bHasGroundSpeed = GroundSpeed > UOD_BaseAnimInstance_Consts::VelocityGroundThreshold;
	
	bShouldMove = bHasVelocity && bHasGroundSpeed;
	bIsFalling = CharacterMovement->IsFalling();
	bIsCrouching = BaseCharacter->bIsCrouched;

	if (const AOD_BaseCharacter* OwnerCharacter = Cast<AOD_BaseCharacter>(BaseCharacter))
	{
		bIsHoldingItem = OwnerCharacter->IsHoldingItem();
	}
}
