// Copyright Epic Games, Inc. All Rights Reserved.

#include "AncientGameMode.h"
#include "AncientCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAncientGameMode::AAncientGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
