// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "AnimNotify_JumpHanging.h"

#include "Ancient/Player/Components/Climb/ClimbComponent.h"
#include "Ancient/Player/AncientCharacter.h"

void UAnimNotify_JumpHanging::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (const AAncientCharacter* Character = Cast<AAncientCharacter>(MeshComp->GetOwner()))
	{
		if(UClimbComponent* ClimbComponent = Character->GetClimbComponent())
		{
			if (ClimbComponent->IsHangingJumpLeft())
			{
				ClimbComponent->Notify_HangingJumpLeft(false);
			}
			else if (ClimbComponent->IsHangingJumpRight())
			{
				ClimbComponent->Notify_HangingJumpRight(false);
			}
			if (ClimbComponent->IsJumpUp())
			{
				ClimbComponent->Notify_JumpUp(false);
			}			
		}
	}
}
