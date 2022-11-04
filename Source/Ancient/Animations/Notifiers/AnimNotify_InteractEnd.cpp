// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "AnimNotify_InteractEnd.h"

#include "Ancient/Player/AncientCharacter.h"

void UAnimNotify_InteractEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AAncientCharacter* Character = Cast<AAncientCharacter>(MeshComp->GetOwner()))
	{
		if (Character->IsInteractingLoopAnim())
		{
			Character->Notify_InteractLoopStop();
		}
		else 
		{
			Character->Notify_InteractStop();
		}
	}
}
