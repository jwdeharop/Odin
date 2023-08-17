#pragma once

#include "CoreMinimal.h"

// Macro to easly access GameTraceChannels.
#define COLLISION_CHANNEL(__NAME__) static_cast<ECollisionChannel>(EOD_CollisionChannels::__NAME__)

enum class EOD_CollisionChannels
{
	InteractRaycast = ECC_GameTraceChannel1
};

namespace OD_CollisionPresets
{
	static const FName OD_Interact = TEXT("OD_Interact");
}