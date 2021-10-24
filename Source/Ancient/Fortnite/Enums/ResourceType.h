// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

UENUM(BlueprintType)
enum class EResourceTypes : uint8
{
	E_Ice	 	UMETA(DisplayName = "Ice"),
	E_Wood	 	UMETA(DisplayName = "Wood"),
	E_Metal 	UMETA(DisplayName = "Metal"),
	E_Stone 	UMETA(DisplayName = "Stone")
};

static FORCEINLINE FName GetEnumValueAsString(const EResourceTypes Value)
{
	switch (Value)
	{
	case EResourceTypes::E_Ice:
		return FName("Ice");
	case EResourceTypes::E_Metal:
		return FName("Metal");
	case EResourceTypes::E_Stone:
		return FName("Stone");
	case EResourceTypes::E_Wood:
		return FName("Wood");
	}
	return FName("None");
} 