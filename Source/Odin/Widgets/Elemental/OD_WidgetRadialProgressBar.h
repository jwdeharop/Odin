#pragma once

#include "Odin.h"
#include "Blueprint/UserWidget.h"
#include "OD_WidgetRadialProgressBar.generated.h"

class UMaterialInterface;
class UMaterialInstanceDynamic;
class UImage;
class UOD_CompInteraction;

UCLASS(config=Game)
class UOD_WdigetRadialProgressBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPercentage(float Percentage) const;

protected:
	UPROPERTY(EditAnywhere)
		UMaterialInterface* MaterialInterface = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		UImage* ImageRound = nullptr;

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(Transient)
		UMaterialInstanceDynamic* DynamicInstance = nullptr;
	UPROPERTY(Transient)
		UOD_CompInteraction* CompInteractionPtr = nullptr;
	
	bool bCanUpdateProgress = false;
	float CurrentProgressDuration = 0.f;

	void HoldInteractionStarts();
	void HoldInteractionEnds();
	void GatherCompInteractionPtr();
};