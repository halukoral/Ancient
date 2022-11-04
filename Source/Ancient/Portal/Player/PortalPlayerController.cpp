// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "PortalPlayerController.h"

#include "PreviewCharacter.h"
#include "Ancient/Ancient.h"
#include "Ancient/Player/AncientCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void APortalPlayerController::SpawnClonePC()
{
	if(ClonePC.IsValid() == false)
	{
		ClonePC = GetWorld()->SpawnActor<APortalPlayerController>();
		ClonePC->Possess(Cast<APawn>(PreviewCharacter));
	}
}

void APortalPlayerController::SpawnCloneCharacter(const FTransform& InPortal, const FTransform& InLinkedPortal)
{
	if(PreviewCharacter.IsValid() == false)
	{
		PreviewCharacter = GetWorld()->SpawnActorDeferred<APreviewCharacter>(APreviewCharacter::StaticClass(), FTransform::Identity);
		if(PreviewCharacter.IsValid())
		{
			PreviewCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(PlayerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
			PreviewCharacter->GetCapsuleComponent()->SetCapsuleRadius(PlayerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius());
			PreviewCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_PortalActor);

			PreviewCharacter->GetMesh()->SetSkeletalMesh(PlayerCharacter->GetMesh()->SkeletalMesh);
			PreviewCharacter->GetMesh()->SetAnimInstanceClass(PlayerCharacter->GetMesh()->AnimClass);
			PreviewCharacter->GetMesh()->SetRelativeTransform(PlayerCharacter->GetMesh()->GetRelativeTransform());

			PreviewCharacter->GetCharacterMovement()->MaxWalkSpeed = PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed;

			PreviewCharacter->FinishSpawning(PlayerCharacter->GetActorTransform());

			LinkedPortal = InLinkedPortal;
			Portal		 = InPortal;

			//FTransform RelativeTransform = PlayerCharacter->GetActorTransform().GetRelativeTransform(BackFacing);
			FTransform RelativeTransform = PlayerCharacter->GetActorTransform().GetRelativeTransform(Portal);

			RelativeTransform *= LinkedPortal;
			
			//const FVector TeleportLocation = (PortalTransform.GetUnitAxis( EAxis::X ) * 10) + RelativeTransform.GetLocation();
			const FVector TeleportLocation = RelativeTransform.GetLocation();
			PreviewCharacter->SetActorLocation(TeleportLocation);

			FRotator TeleportRotation = RelativeTransform.Rotator();
			TeleportRotation.Roll = 0;
			PreviewCharacter->SetActorRotation(TeleportRotation);

			SpawnClonePC();
		}
	}
}

void APortalPlayerController::DestroyPreviewCharacter()
{
	ClonePC->UnPossess();
	if(PreviewCharacter.IsValid())
	{
		PreviewCharacter->Destroy();
		PreviewCharacter.Reset();
	}
}

void APortalPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<AAncientCharacter>(GetPawn());
}

void APortalPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &APortalPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APortalPlayerController::MoveRight);

	InputComponent->BindAxis("TurnRate", this, &APortalPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUpRate", this, &APortalPlayerController::LookUpAtRate);
}

void APortalPlayerController::TurnAtRate(float Rate)
{
	if(PreviewCharacter.IsValid())
	{
		PreviewCharacter->TurnAtRate(Rate);
	}

	if(PlayerCharacter.IsValid())
	{
		PlayerCharacter->TurnAtRate(Rate);
	}
}

void APortalPlayerController::LookUpAtRate(float Rate)
{
	if(PreviewCharacter.IsValid())
	{
		PreviewCharacter->LookUpAtRate(Rate);
	}

	if(PlayerCharacter.IsValid())
	{
		PlayerCharacter->LookUpAtRate(Rate);
	}
}

void APortalPlayerController::MoveForward(float Value)
{
	if(Value == 0.0f)
	{
		return;
	}
	
	if(false && PreviewCharacter.IsValid())
	{
		//ClonePC->SetControlRotation(GetControlRotation());
		ClonePC->SetControlRotation(GetControlRotation());
		PreviewCharacter->MoveForward(Value);
	}

	if(PlayerCharacter.IsValid())
	{
		PlayerCharacter->MoveForward(Value);
	}
}

void APortalPlayerController::MoveRight(float Value)
{
	if(Value == 0.0f)
	{
		return;
	}
	
	if(false && PreviewCharacter.IsValid())
	{
		//ClonePC->SetControlRotation(GetControlRotation());
		ClonePC->SetControlRotation(GetControlRotation());
		PreviewCharacter->MoveRight(Value);
	}

	if(PlayerCharacter.IsValid())
	{
		PlayerCharacter->MoveRight(Value);
	}
}
