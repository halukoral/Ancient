// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "AnimNotify_TurnHanging.h"

#include "Ancient/Player/Components/Climb/ClimbComponent.h"
#include "Ancient/Player/AncientCharacter.h"

void UAnimNotify_TurnHanging::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (const AAncientCharacter* Character = Cast<AAncientCharacter>(MeshComp->GetOwner()))
	{
		if (Character->GetClimbComponent()->IsHangingCornerLeft())
		{
			Character->GetClimbComponent()->Notify_HangingCornerLeft(false);
		}
		else if (Character->GetClimbComponent()->IsHangingCornerRight())
		{
			Character->GetClimbComponent()->Notify_HangingCornerRight(false);
		}
	}
}
