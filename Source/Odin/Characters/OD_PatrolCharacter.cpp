#include "OD_PatrolCharacter.h"

#include "Components/SplineComponent.h"

AOD_PatrolCharacter::AOD_PatrolCharacter() : Super()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);

#if WITH_EDITOR
	SplineComponent->bDrawDebug = true;
#endif
}

void AOD_PatrolCharacter::FillSplinePoints()
{
	for (int32 Index = 0; Index <= SplineComponent->GetNumberOfSplinePoints(); ++Index)
	{
		SplinePoints.Add(SplineComponent->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World));
	}
}

const TArray<FVector>& AOD_PatrolCharacter::GetSplinePoints() const
{
	return SplinePoints;
}

bool AOD_PatrolCharacter::IsUsingTorch() const
{
	return bIsUsingTorch;
}

void AOD_PatrolCharacter::BeginPlay()
{
	Super::BeginPlay();

	FillSplinePoints();
}
