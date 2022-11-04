// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class ACharacter;

UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType, meta = (BlueprintThreadSafe), Within = SkeletalMeshComponent)
class ANCIENT_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	/** use in 1D Blend Space */
	UPROPERTY(BlueprintReadWrite)
	float Speed	= 0.f;

	UPROPERTY(BlueprintReadWrite)
	float Pitch = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float Yaw = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float Time = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float Direction = 0.f;

	FRotator Current = FRotator::ZeroRotator;
	
	FRotator ControlRotation = FRotator::ZeroRotator;

	FRotator ActorRotation = FRotator::ZeroRotator;

	FRotator Target = FRotator::ZeroRotator;

	FRotator InterpRotator = FRotator::ZeroRotator;

	TWeakObjectPtr<ACharacter> Character = nullptr;
};
