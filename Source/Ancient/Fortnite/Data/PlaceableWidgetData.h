// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "Engine/DataTable.h"
#include "Ancient/Fortnite/Enums/PlaceableType.h"
#include "PlaceableWidgetData.generated.h"

USTRUCT(BlueprintType)
struct FPlaceableWidgetData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	EPlaceableType Type;

	UPROPERTY(BlueprintReadWrite)
	FText Name;

	UPROPERTY(BlueprintReadWrite)
	FKey Hotkey;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* Icon;
};