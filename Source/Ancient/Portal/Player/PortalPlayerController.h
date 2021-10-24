// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PortalPlayerController.generated.h"

class AAncientCharacter;
class APreviewCharacter;

UCLASS()
class ANCIENT_API APortalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	void 		 SpawnClonePC();

	void 		 SpawnCloneCharacter(const FTransform& InPortal, const FTransform& InLinkedPortal);

	void 		 DestroyPreviewCharacter();

protected:
	
	virtual void BeginPlay() override;
	
	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;
	
	void		 MoveRight(float Value);

	void		 MoveForward(float Value);

	void		 TurnAtRate(float Rate);

	void		 LookUpAtRate(float Rate);

private:

	FTransform LinkedPortal	= FTransform::Identity;

	FTransform Portal		= FTransform::Identity;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	float	BaseTurnRate	= 45.f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	float	BaseLookUpRate	= 45.f;
	
	TWeakObjectPtr<APortalPlayerController> ClonePC				= nullptr;

	TWeakObjectPtr<AAncientCharacter>		PlayerCharacter		= nullptr;

	TWeakObjectPtr<APreviewCharacter>		PreviewCharacter	= nullptr;
	
};
