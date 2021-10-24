// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "AnimNotify_Climb.h"

#include "Ancient/Player/Components/Climb/ClimbComponent.h"
#include "Ancient/Player/AncientCharacter.h"

void UAnimNotify_Climb::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AAncientCharacter* Character = Cast<AAncientCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->GetClimbComponent()->Notify_HangingFinished();
	}
}
