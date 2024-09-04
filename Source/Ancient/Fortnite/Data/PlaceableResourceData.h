#pragma once

#include "Engine/DataTable.h"
#include "Ancient/Fortnite/Enums/ResourceType.h"
#include "PlaceableResourceData.generated.h"

USTRUCT(BlueprintType)
struct FPlaceableResourceData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EResourceTypes ResourceType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RequiredAmount = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Time = 2.0f;
};
