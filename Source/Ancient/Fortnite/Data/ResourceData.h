#pragma once

#include "Engine/DataTable.h"
#include "Ancient/Fortnite/Enums/ResourceType.h"
#include "ResourceData.generated.h"

USTRUCT(BlueprintType)
struct FResourceData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EResourceTypes Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D*	Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor GhostBuildingColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInstance* MeshMaterial;
};
