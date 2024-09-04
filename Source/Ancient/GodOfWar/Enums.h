#pragma once

UENUM(BlueprintType)
enum class EMarkerType : uint8
{
	E_Landmark	UMETA(DisplayName = "Landmark"),
	E_Quest		UMETA(DisplayName = "Quest")
};

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	E_Main	UMETA(DisplayName = "Main Quest"),
	E_Side	UMETA(DisplayName = "Side Quest")
};