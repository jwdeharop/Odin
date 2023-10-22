#pragma once

#include "CoreMinimal.h"
#include "OD_InteractionInterface.generated.h"

class UOD_CompInteractable;

UINTERFACE(MinimalAPI, Blueprintable)
class UOD_InteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class IOD_InteractionInterface
{
	GENERATED_BODY()

public:

	virtual UOD_CompInteractable* GetCompInteractable () { return nullptr; }
	virtual void PrepareInteraction(bool bCanInteract) {};
	virtual void StartInteraction(ACharacter* Instigator) {};
	virtual void CancelInteraction() {};
	virtual void InteractionSuccessful() {};
};