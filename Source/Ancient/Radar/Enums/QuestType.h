// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	E_Main	UMETA(DisplayName = "Main Quest"),
	E_Side	UMETA(DisplayName = "Side Quest")
};