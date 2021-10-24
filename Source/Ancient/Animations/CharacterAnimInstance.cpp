// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "CharacterAnimInstance.h"
#include "GameFramework/Character.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character.IsValid())
	{
		Time = DeltaSeconds;

		Speed = (Character->GetVelocity() * FVector(1.0f, 1.0f, 0.f)).Size();
		
		/* ------- Set Yaw and Pitch ------- */
		Current = FRotator(0.f, Pitch, Yaw);

		ControlRotation = Character->GetControlRotation();

		ActorRotation = Character->GetActorRotation();

		FRotator Delta = ControlRotation - ActorRotation;
		Delta.Normalize();
		Target = Delta;
		
		InterpRotator = FMath::RInterpTo(Current, Target, DeltaSeconds, 0.f);

		Pitch = FMath::ClampAngle(InterpRotator.Pitch, -90.f, 90.f);

		Yaw = FMath::ClampAngle(InterpRotator.Yaw, -90.f, 90.f);
		/* ------- ~Set Yaw and Pitch~ ------- */

		Direction = CalculateDirection(Character->GetVelocity(), ActorRotation);
	}
}
