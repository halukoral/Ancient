// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "AnimNotify_Climb.h"

#include "Ancient/Player/Components/Climb/ClimbComponent.h"
#include "Ancient/Player/AncientCharacter.h"

void UAnimNotify_Climb::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (const AAncientCharacter* Character = Cast<AAncientCharacter>(MeshComp->GetOwner()))
	{
		if(UClimbComponent* ClimbComponent = Character->GetClimbComponent())
		{
			ClimbComponent->Notify_HangingFinished();			
		}
	}
}
