#pragma once

UENUM(BlueprintType)
enum class EProgressState : uint8
{
	E_Processing	UMETA(DisplayName = "Processing"),
	E_Finished		UMETA(DisplayName = "Finished"),
};