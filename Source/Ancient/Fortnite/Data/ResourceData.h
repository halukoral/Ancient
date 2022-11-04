// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "Engine/DataTable.h"
#include "Ancient/Fortnite/Enums/ResourceType.h"
#include "ResourceData.generated.h"

USTRUCT(BlueprintType)
struct FResourceData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FText Name;

	UPROPERTY(BlueprintReadWrite)
	EResourceTypes Type;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D*	Icon;

	UPROPERTY(BlueprintReadWrite)
	FLinearColor GhostBuildingColor;

	UPROPERTY(BlueprintReadWrite)
	UMaterial* MeshMaterial;
};