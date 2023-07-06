#include "OD_PatrolAIController.h"
#include "Perception/AISenseConfig_Sight.h"

AOD_PatrolAIController::AOD_PatrolAIController() : Super()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
}
