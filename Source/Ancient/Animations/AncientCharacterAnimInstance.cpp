// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "AncientCharacterAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Ancient/Player/Components/Climb/ClimbComponent.h"
#include "Ancient/Player/AncientCharacter.h"

void UAncientCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (const AAncientCharacter* AncientCharacter = Cast<AAncientCharacter>(TryGetPawnOwner()))
	{
		if(const UPawnMovementComponent* MovementComponent = AncientCharacter->GetMovementComponent())
		{
			bIsCrouching = MovementComponent->IsCrouching();
			bIsInAir = MovementComponent->IsFalling();
			bIsFlying = MovementComponent->IsFlying() && !bIsHanging && !bIsClimbing;
		}

		bIsInteracting = AncientCharacter->IsInteracting();
		bIsInteractingLoop = AncientCharacter->IsInteractingLoop();
		bIsCelebrating = AncientCharacter->IsCelebrating();
		
		if(const UClimbComponent* ClimbComponent = AncientCharacter->GetClimbComponent())
		{
			/* ------- Hanging ------- */
			bIsHanging = ClimbComponent->IsHanging();

			bIsClimbing = ClimbComponent->IsClimbing();

			bIsJumpingUp = ClimbComponent->IsJumpUp();

			bIsJumpBack = ClimbComponent->IsJumpBack();
				
			bIsHangingLeft = ClimbComponent->IsHangingLeft();

			bIsHangingRight = ClimbComponent->IsHangingRight();

			bIsHangingJumpLeft = ClimbComponent->IsHangingJumpLeft();

			bIsHangingJumpRight = ClimbComponent->IsHangingJumpRight();

			bIsHangingCornerLeft = ClimbComponent->IsHangingCornerLeft();

			bIsHangingCornerRight = ClimbComponent->IsHangingCornerRight();
			/* ------- ~Hanging~ ------- */
		}
	}
}
