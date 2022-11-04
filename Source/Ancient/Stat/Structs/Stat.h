// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "Stat.generated.h"

USTRUCT()
struct FStat
{
	GENERATED_BODY()
	
	int32 Min = 0;
	int32 Current = 0;
	int32 Max = 0;
	
	FStat()
	{
		Min = 0;
		Current = 0;
		Max = 100;
	}

	FStat(const int32 InMin, const int32 InCurrent, const int32 InMax)
	{
		Min = InMin;
		Current	= InCurrent;
		Max = InMax;
	}
};