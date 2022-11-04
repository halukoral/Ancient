// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimInstance.h"
#include "AncientCharacterAnimInstance.generated.h"

class AAncientCharacter;

UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType, meta = (BlueprintThreadSafe), Within = SkeletalMeshComponent)
class ANCIENT_API UAncientCharacterAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

public:
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(BlueprintReadWrite)
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCrouching = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsFlying = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCelebrating	= false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsInteractingLoop	= false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsInteracting = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsJumpingUp = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsJumpBack = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHanging	= false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsClimbing = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHangingLeft = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHangingRight = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHangingJumpLeft = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHangingJumpRight = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHangingCornerLeft = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHangingCornerRight = false;

	TWeakObjectPtr<AAncientCharacter> AncientCharacter = nullptr;
};
