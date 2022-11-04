// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "Engine/DataTable.h"
#include "Ancient/Fortnite/Enums/ResourceType.h"
#include "PlaceableResourceData.generated.h"

USTRUCT(BlueprintType)
struct FPlaceableResourceData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	EResourceTypes ResourceType;

	UPROPERTY(BlueprintReadWrite)
	int32 RequiredAmount = 10;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxHealth = 100;

	UPROPERTY(BlueprintReadWrite)
	float Time = 2.0f;
};