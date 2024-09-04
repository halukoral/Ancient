#pragma once

UENUM(BlueprintType)
enum class EPlaceableType : uint8
{
	E_Wall	 	UMETA(DisplayName = "Wall"),
	E_Stair	 	UMETA(DisplayName = "Stair"),
	E_Floor 	UMETA(DisplayName = "Floor")
};