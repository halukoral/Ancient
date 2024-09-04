#pragma once

#include "Engine/DataTable.h"
#include "Ancient/Fortnite/Enums/PlaceableType.h"
#include "Ancient/Fortnite/Enums/ResourceType.h"
#include "PlaceableMeshData.generated.h"

USTRUCT(BlueprintType)
struct FPlaceableMeshData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector LocationOffset = FVector(0.f, 0.f, 0.f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPlaceableType PlaceableType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EResourceTypes ResourceType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* Mesh;
};
