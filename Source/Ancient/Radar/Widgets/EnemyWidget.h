// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EnemyWidget.generated.h"

UCLASS()
class ANCIENT_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateDistance(const int32 Distance) const;

private:

	int32 MaxDisplayedDistance	= 999;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DistanceText = nullptr;
	
};
