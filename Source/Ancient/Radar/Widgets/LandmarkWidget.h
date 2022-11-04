// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LandmarkWidget.generated.h"

class UImage;

UCLASS()
class ANCIENT_API ULandmarkWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetLandmarkIcon(UTexture2D* NewIcon) { LandmarkIcon = NewIcon; }
	
protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage*	Icon = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D*	LandmarkIcon = nullptr;

};
