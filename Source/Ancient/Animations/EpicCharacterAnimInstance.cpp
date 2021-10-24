// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md


#include "EpicCharacterAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Ancient/Player/Components/Climb/ClimbComponent.h"
#include "Ancient/Player/AncientCharacter.h"

void UEpicCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<AAncientCharacter>(TryGetPawnOwner());
}

void UEpicCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character)
	{
		Time = DeltaSeconds;

		Speed = (Character->GetVelocity() * FVector(1.0f, 1.0f, 0.f)).Size();
		
		/* ------- Set Yaw and Pitch ------- */
		Current = FRotator(0.f, Pitch, Yaw);

		ControlRotation = Character->GetControlRotation();

		ActorRotation = Character->GetActorRotation();

		Target = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, ActorRotation);

		InterpRotator = FMath::RInterpTo(Current, Target, DeltaSeconds, 0.f);

		Pitch = FMath::ClampAngle(InterpRotator.Pitch, -90.f, 90.f);

		Yaw = FMath::ClampAngle(InterpRotator.Yaw, -90.f, 90.f);
		/* ------- ~Set Yaw and Pitch~ ------- */

		Direction = CalculateDirection(Character->GetVelocity(), ActorRotation);

		/* ------- Hanging ------- */
		bIsHanging = Character->GetClimbComponent()->IsHanging();

		bIsClimbing = Character->GetClimbComponent()->IsClimbing();

		bIsJumpingUp = Character->GetClimbComponent()->IsJumpUp();

		bIsJumpBack = Character->GetClimbComponent()->IsJumpBack();
			
		bIsHangingLeft = Character->GetClimbComponent()->IsHangingLeft();

		bIsHangingRight = Character->GetClimbComponent()->IsHangingRight();

		bIsHangingJumpLeft = Character->GetClimbComponent()->IsHangingJumpLeft();

		bIsHangingJumpRight = Character->GetClimbComponent()->IsHangingJumpRight();

		bIsHangingCornerLeft = Character->GetClimbComponent()->IsHangingCornerLeft();

		bIsHangingCornerRight = Character->GetClimbComponent()->IsHangingCornerRight();
		/* ------- ~Hanging~ ------- */

		bIsInAir = Character->GetMovementComponent()->IsFalling();

		bIsFlying = Character->GetMovementComponent()->IsFlying() && !bIsHanging && !bIsClimbing;

		bIsCrouching = Character->GetMovementComponent()->IsCrouching();

		bIsInteracting = Character->IsInteracting();

		bIsInteractingLoop = Character->IsInteractingLoop();

		bIsCelebrating = Character->IsCelebrating();

	}
}
