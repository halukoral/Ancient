#pragma once

UENUM(BlueprintType)
enum class EPlayerMode : uint8
{
	E_Combat 		UMETA(DisplayName = "Combat"),
	E_Construction  UMETA(DisplayName = "Construction")
};
