// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "AnimNotify_JumpHanging.h"

#include "Ancient/Player/Components/Climb/ClimbComponent.h"
#include "Ancient/Player/AncientCharacter.h"

void UAnimNotify_JumpHanging::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (const AAncientCharacter* Character = Cast<AAncientCharacter>(MeshComp->GetOwner()))
	{
		if (Character->GetClimbComponent()->IsHangingJumpLeft())
		{
			Character->GetClimbComponent()->Notify_HangingJumpLeft(false);
		}
		else if (Character->GetClimbComponent()->IsHangingJumpRight())
		{
			Character->GetClimbComponent()->Notify_HangingJumpRight(false);
		}
		if (Character->GetClimbComponent()->IsJumpUp())
		{
			Character->GetClimbComponent()->Notify_JumpUp(false);
		}
	}
}
