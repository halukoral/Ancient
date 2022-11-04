// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "AncientCharacterAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Ancient/Player/Components/Climb/ClimbComponent.h"
#include "Ancient/Player/AncientCharacter.h"

void UAncientCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AncientCharacter = Cast<AAncientCharacter>(TryGetPawnOwner());
}

void UAncientCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AncientCharacter.IsValid())
	{
		/* ------- Hanging ------- */
		bIsHanging = AncientCharacter->GetClimbComponent()->IsHanging();

		bIsClimbing = AncientCharacter->GetClimbComponent()->IsClimbing();

		bIsJumpingUp = AncientCharacter->GetClimbComponent()->IsJumpUp();

		bIsJumpBack = AncientCharacter->GetClimbComponent()->IsJumpBack();
			
		bIsHangingLeft = AncientCharacter->GetClimbComponent()->IsHangingLeft();

		bIsHangingRight = AncientCharacter->GetClimbComponent()->IsHangingRight();

		bIsHangingJumpLeft = AncientCharacter->GetClimbComponent()->IsHangingJumpLeft();

		bIsHangingJumpRight = AncientCharacter->GetClimbComponent()->IsHangingJumpRight();

		bIsHangingCornerLeft = AncientCharacter->GetClimbComponent()->IsHangingCornerLeft();

		bIsHangingCornerRight = AncientCharacter->GetClimbComponent()->IsHangingCornerRight();
		/* ------- ~Hanging~ ------- */

		bIsInAir = AncientCharacter->GetMovementComponent()->IsFalling();

		bIsFlying = AncientCharacter->GetMovementComponent()->IsFlying() && !bIsHanging && !bIsClimbing;

		bIsCrouching = AncientCharacter->GetMovementComponent()->IsCrouching();

		bIsInteracting = AncientCharacter->IsInteracting();

		bIsInteractingLoop = AncientCharacter->IsInteractingLoop();

		bIsCelebrating = AncientCharacter->IsCelebrating();
	}
}
