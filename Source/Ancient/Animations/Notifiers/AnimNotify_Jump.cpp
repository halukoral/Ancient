// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "AnimNotify_Jump.h"

#include "Ancient/Player/AncientCharacter.h"

void UAnimNotify_Jump::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AAncientCharacter* Character = Cast<AAncientCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->Notify_JumpFinished();
	}
}
