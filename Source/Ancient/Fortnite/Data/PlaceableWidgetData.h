#pragma once

#include "Engine/DataTable.h"
#include "Ancient/Fortnite/Enums/PlaceableType.h"
#include "PlaceableWidgetData.generated.h"

USTRUCT(BlueprintType)
struct FPlaceableWidgetData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPlaceableType Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FKey Hotkey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Icon;
};
