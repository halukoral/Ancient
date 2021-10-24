// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EpicCharacterAnimInstance.generated.h"

class AAncientCharacter;

UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType, meta = (BlueprintThreadSafe), Within = SkeletalMeshComponent)
class ANCIENT_API UEpicCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	UEpicCharacterAnimInstance() {};

	virtual void	NativeInitializeAnimation() override;
	
	virtual void	NativeUpdateAnimation(float DeltaSeconds) override;

private:

	/** use in 1D Blend Space */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float				Speed					= 0.f;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float				Pitch					= 0.f;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float				Yaw						= 0.f;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float				Time					= 0.f;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float				Direction				= 0.f;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsInAir				= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsCrouching			= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsFlying				= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsCelebrating			= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsInteractingLoop		= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsInteracting			= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsJumpingUp			= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsJumpBack				= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsHanging				= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsClimbing				= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsHangingLeft			= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsHangingRight			= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsHangingJumpLeft		= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsHangingJumpRight		= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsHangingCornerLeft	= false;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool				bIsHangingCornerRight	= false;

	FRotator			Current					= FRotator::ZeroRotator;
	
	FRotator			ControlRotation			= FRotator::ZeroRotator;

	FRotator			ActorRotation			= FRotator::ZeroRotator;

	FRotator			Target					= FRotator::ZeroRotator;

	FRotator			InterpRotator			= FRotator::ZeroRotator;

	AAncientCharacter*	Character				= nullptr;
};
