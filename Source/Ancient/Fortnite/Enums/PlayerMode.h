// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

UENUM(BlueprintType)
enum class EPlayerMode : uint8
{
	E_Combat 		UMETA(DisplayName = "Combat"),
	E_Construction  UMETA(DisplayName = "Construction")
};
