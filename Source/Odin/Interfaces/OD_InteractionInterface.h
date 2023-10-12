#pragma once

#include "CoreMinimal.h"
#include "OD_InteractionInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UOD_InteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class IOD_InteractionInterface
{
	GENERATED_BODY()

public:
	virtual void PrepareInteraction(bool bCanInteract) {};
	virtual void StartInteraction(ACharacter* Instigator) {};
	virtual void CancelInteraction() {};
	virtual void InteractionSuccessful() {};
	virtual bool IsHoldInteraction() { return false; }
};