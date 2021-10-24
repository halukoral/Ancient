// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Controllable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UControllable : public UInterface
{
	GENERATED_BODY()
};

class ANCIENT_API IControllable
{
	GENERATED_BODY()

public:

	virtual void SetupInput(class UInputComponent* PlayerInputComponent) PURE_VIRTUAL(IControllable::SetupInput,);
};
