// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "Engine/DataTable.h"
#include "Ancient/Fortnite/Enums/PlaceableType.h"
#include "Ancient/Fortnite/Enums/ResourceType.h"
#include "PlaceableMeshData.generated.h"

USTRUCT(BlueprintType)
struct FPlaceableMeshData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	EPlaceableType PlaceableType;

	UPROPERTY(BlueprintReadWrite)
	EResourceTypes ResourceType;

	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* Mesh;
};