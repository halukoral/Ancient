// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "AnimNotify_Interact.h"

#include "Ancient/Player/AncientCharacter.h"

void UAnimNotify_Interact::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AAncientCharacter* Character = Cast<AAncientCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		if (Character->IsInteractingLoopAnim())
		{
			Character->Notify_InteractLoopExecute();
		}
		else
		{
			Character->Notify_InteractExecute();
		}
	}
}
