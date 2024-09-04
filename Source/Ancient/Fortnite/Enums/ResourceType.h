#pragma once

UENUM(BlueprintType)
enum class EResourceTypes : uint8
{
	E_Ice	 	UMETA(DisplayName = "Ice"),
	E_Wood	 	UMETA(DisplayName = "Wood"),
	E_Stone 	UMETA(DisplayName = "Stone"),
	E_Count		UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EResourceTypes, EResourceTypes::E_Count);

static FORCEINLINE FName GetEnumValueAsString(const EResourceTypes Value)
{
	switch (Value)
	{
	case EResourceTypes::E_Ice:
		return FName("Ice");
	case EResourceTypes::E_Wood:
		return FName("Wood");
	case EResourceTypes::E_Stone:
		return FName("Stone");
	}
	return FName("None");
} 