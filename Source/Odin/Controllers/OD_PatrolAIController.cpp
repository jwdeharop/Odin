#include "OD_PatrolAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

namespace AOD_PatrolAIController_Consts
{
	constexpr float SightRadius = 1000.f;
	constexpr float LoseSightRadius = 20.f;
	constexpr float PeripheralVisionAngelDegrees = 45.f;
}

AOD_PatrolAIController::AOD_PatrolAIController() : Super()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = AOD_PatrolAIController_Consts::SightRadius;
	SightConfig->LoseSightRadius = SightConfig->LoseSightRadius + AOD_PatrolAIController_Consts::LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AOD_PatrolAIController_Consts::PeripheralVisionAngelDegrees;

	constexpr bool bDetect = true;
	constexpr bool bNoDetect = false;

	SightConfig->DetectionByAffiliation.bDetectEnemies = bDetect;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = bNoDetect;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = bNoDetect;

	if (PerceptionComponent)
	{
		PerceptionComponent->ConfigureSense(*SightConfig);
		PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	}
}
