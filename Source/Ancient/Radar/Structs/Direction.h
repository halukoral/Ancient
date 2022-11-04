// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Direction.generated.h"

USTRUCT(BlueprintType)
struct FDirection
{
	GENERATED_BODY()

	FDirection() : Name(TEXT("No Name")) , WorldRotation(0) { }
	FDirection(const FString InName, const int32 InRotation) : Name(InName), WorldRotation(InRotation) { }
	
	FString Name;
	int32 WorldRotation;
};