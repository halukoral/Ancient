// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "Portal.h"

#include "Ancient/Utils/DebuggerLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Player/PortalPlayerController.h"

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = Scene;
	
	BackFacingScene = CreateDefaultSubobject<USceneComponent>("BackFacingScene");
	BackFacingScene->SetupAttachment(Scene);

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("Scene Capture");
	SceneCapture->SetupAttachment(RootComponent);
	SceneCapture->bOverride_CustomNearClippingPlane = true;
	SceneCapture->CustomNearClippingPlane = 100.f;
	
	Portal = CreateDefaultSubobject<UStaticMeshComponent>("Portal");
	Portal->SetupAttachment(RootComponent);
	Portal->SetCollisionProfileName("Portal");
	Portal->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnTeleportEnter);
	Portal->OnComponentEndOverlap.AddDynamic(this, &APortal::OnTeleportExit);
	
	PortalRing = CreateDefaultSubobject<UStaticMeshComponent>("Portal Ring");
	PortalRing->SetupAttachment(RootComponent);
	PortalRing->SetCollisionProfileName("Portal");

	MI_Blue   = CreateDefaultSubobject<UMaterialInstance>("MI Blue");
	MI_Yellow = CreateDefaultSubobject<UMaterialInstance>("MI Yellow");

	MI_RT_Blue   = CreateDefaultSubobject<UMaterialInstance>("RT Blue");
	MI_RT_Yellow = CreateDefaultSubobject<UMaterialInstance>("RT Yellow");

	RT_Blue   = CreateDefaultSubobject<UTextureRenderTarget2D>("RT Blue");
	RT_Yellow = CreateDefaultSubobject<UTextureRenderTarget2D>("RT Yellow");

}

void APortal::BeginPlay()
{
	Super::BeginPlay();

	if(PortalType == EPortalType::E_Blue)
	{
		PortalRing->SetMaterial(0, MI_Blue);
		SceneCapture->TextureTarget = RT_Blue;
	}
	else
	{
		PortalRing->SetMaterial(0, MI_Yellow);
		SceneCapture->TextureTarget = RT_Yellow;
	}

	// TODO: Use this code every time when screen resolution changed
	PC = Cast<APortalPlayerController>(GetWorld()->GetFirstPlayerController());
	if(PC.IsValid())
	{
		int32 X,Y;
		PC->GetViewportSize(X,Y);
		SceneCapture->TextureTarget->ResizeTarget(X * PortalQuality,Y * PortalQuality);		
	}

	bCanEnter = true;
	bCanExit  = true;
}

void APortal::OnTeleportEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(LinkedPortal.IsValid() && OtherActor != this)
	{
		if(CanTeleport(OtherActor))
		{
			CharacterToTeleport = Cast<ACharacter>(OtherActor);
			if(CharacterToTeleport.IsValid() && PC.IsValid() && CharacterToTeleport->IsLocallyControlled())
			{
				CharacterToTeleport->GetCapsuleComponent()->SetCollisionObjectType(ECC_PortalActor);
				PC->SpawnCloneCharacter(GetActorTransform(), LinkedPortal->GetActorTransform());

				// if(Character->GetCapsuleComponent()->GetCollisionProfileName() != "ActorInPortal")
				// {
				// 	ActorDefaultCollision = Character->GetCapsuleComponent()->GetCollisionProfileName();
				// 	LOG(Character->GetCapsuleComponent()->GetCollisionProfileName().ToString());
				// 	Character->GetCapsuleComponent()->SetCollisionProfileName("ActorInPortal", false);
				// 	LOG(ActorDefaultCollision.ToString());
				// 	LOG(Character->GetCapsuleComponent()->GetCollisionProfileName().ToString());
				// }

				//Teleport(Character);
			}
			else if(OtherActor->GetRootComponent()->IsAnySimulatingPhysics())
			{
				
			}
		}
	}
}

void APortal::OnTeleportExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(CharacterToTeleport.IsValid())
	{
		
	}
	
	if(LinkedPortal.IsValid() && OtherActor != this && bCanExit)
	{
		CharacterToTeleport = Cast<ACharacter>(OtherActor);
		if(CharacterToTeleport.IsValid())
		{
			LOGM("OnTeleportExit %s", *GetName());
			LinkedPortal->bCanEnter = true;
			LinkedPortal->bCanExit = true;

			CharacterToTeleport->GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
			
			// LOG(LinkedPortal->ActorDefaultCollision.ToString());
			// Character->GetCapsuleComponent()->SetCollisionProfileName(LinkedPortal->ActorDefaultCollision, false);
			// LinkedPortal->ActorDefaultCollision = FName("None");
			// LOGM("Name %s ---- %s", *OtherActor->GetName(), *GetName());

			LOGM("bCanEnter %d", bCanEnter);
			LOGM("bCanExit %d", bCanExit);
			LOGM("LinkedPortal->bCanEnter %d", LinkedPortal->bCanEnter);
			LOGM("LinkedPortal->bCanExit %d", LinkedPortal->bCanExit);
		}
	}
}

void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(LinkedPortal.IsValid() && PC.IsValid() && PC->PlayerCameraManager)
	{		
		const FTransform Transform = PC->PlayerCameraManager->GetTransformComponent()->GetComponentTransform();
		const FTransform RelativeTransform = Transform.GetRelativeTransform(BackFacingScene->GetComponentTransform());
		LinkedPortal->SceneCapture->SetRelativeLocationAndRotation(RelativeTransform.GetLocation(), RelativeTransform.GetRotation());

		const float Dist = FVector::Dist(Transform.GetLocation(), GetActorLocation()) + 1.f;
		LinkedPortal->SceneCapture->CustomNearClippingPlane = Dist;
	}
}

void APortal::Link(APortal* Other)
{
	LinkedPortal = Other;

	if(LinkedPortal->LinkedPortal == nullptr)
	{
		LinkedPortal->Link(this);		
	}
	
	if(PortalType == EPortalType::E_Blue)
	{
		Portal->SetMaterial(0, MI_RT_Yellow);
	}
	else
	{
		Portal->SetMaterial(0, MI_RT_Blue);
	}
}

bool APortal::CanTeleport(const AActor* Actor) const
{
	FVector ActorVelocity = Actor->GetVelocity();
	const FVector Forward = Actor->GetActorLocation() + (ActorVelocity * GetWorld()->GetDeltaSeconds());
	FVector NormalizedVector = Forward - GetActorLocation();

	NormalizedVector.Normalize();
	ActorVelocity.Normalize();

	const float LocationDotProduct = FVector::DotProduct( NormalizedVector , GetActorForwardVector() );
	const float VelocityDotProduct = FVector::DotProduct( ActorVelocity , GetActorForwardVector() );

	// LOGM("LocationDotProduct: %f", LocationDotProduct);
	// LOGM("VelocityDotProduct: %f", VelocityDotProduct);
	return LocationDotProduct > 0.f && bCanEnter && LinkedPortal->bCanEnter;
}

void APortal::Teleport(AActor* Actor)
{
	bCanEnter = false;
	bCanExit  = false;
}

void APortal::Teleport(ACharacter* Character)
{
	LOGM("Teleport %s", *GetName());
	bCanEnter = false;
	bCanExit  = false;
	FTransform RelativeTransform = Character->GetActorTransform().GetRelativeTransform(BackFacingScene->GetComponentTransform());
	RelativeTransform *= LinkedPortal->GetActorTransform();

	const FVector TeleportLocation = (LinkedPortal->GetActorForwardVector() * 10) + RelativeTransform.GetLocation();
	//TeleportLocation -= Camera->GetRelativeLocation();
	Character->SetActorLocation(TeleportLocation);
			
	FRotator TeleportRotation = RelativeTransform.Rotator();
	TeleportRotation.Roll = 0;
	Character->GetController()->SetControlRotation(TeleportRotation);
	Character->SetActorRotation(TeleportRotation);
			
	const FVector InverseDirection = Character->GetActorTransform().InverseTransformPositionNoScale(Character->GetVelocity());
	const FTransform TransformForVelocity = FTransform( Character->GetController()->GetControlRotation(),Character->GetActorLocation());
	Character->GetMovementComponent()->Velocity = TransformForVelocity.TransformVectorNoScale(InverseDirection);
	Character->GetMovementComponent()->UpdateComponentVelocity();
}