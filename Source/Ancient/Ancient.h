// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAncient, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogFortnite, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogGodOfWar, All, All);

// Look: DefaultEngine.ini
static ETraceTypeQuery	 ClimbableTraceQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
static ECollisionChannel ECC_Portal = ECollisionChannel::ECC_GameTraceChannel2;
static ECollisionChannel ECC_PortalWall = ECollisionChannel::ECC_GameTraceChannel3;
static ECollisionChannel ECC_PortalActor = ECollisionChannel::ECC_GameTraceChannel4;
