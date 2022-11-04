// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "AnimNotify_Celebrate.h"

#include "Ancient/Player/AncientCharacter.h"

void UAnimNotify_Celebrate::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AAncientCharacter* Character = Cast<AAncientCharacter>(MeshComp->GetOwner()))
	{
		Character->Notify_CelebratingFinished(false);
	}
}
