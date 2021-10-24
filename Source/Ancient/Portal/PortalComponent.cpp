// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "PortalComponent.h"

#include "Portal.h"
#include "PortalWall.h"
#include "Ancient/Ancient.h"
#include "GameFramework/Character.h"

UPortalComponent::UPortalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPortalComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());

	if(Character == nullptr || (Character.Get() && Character->GetController()->IsLocalPlayerController() == false ))
	{
		DestroyComponent();
	}
}

void UPortalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPortalComponent::SetupInput(UInputComponent* PlayerInputComponent)
{
	DECLARE_DELEGATE_OneParam(FCustomInputDelegate, const EPortalType);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("Portal_Spawn_Blue", IE_Pressed, this, &UPortalComponent::Action_SpawnPortal, EPortalType::E_Blue);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("Portal_Spawn_Yellow", IE_Pressed, this, &UPortalComponent::Action_SpawnPortal, EPortalType::E_Yellow);
}

void UPortalComponent::Action_SpawnPortal(const EPortalType Type)
{
	FVector Start;
	FRotator ViewPointRotation;
	Character->GetController()->GetPlayerViewPoint(
		Start,
		ViewPointRotation
	);
	const FVector End = Start + (ViewPointRotation.Vector() * TraceDistance);

	FHitResult OutHit;
	FCollisionQueryParams Params;
	FCollisionObjectQueryParams ObjectParams = ECC_PortalWall;

	if(GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParams, Params))
	{
		if( APortalWall* PortalWall = Cast<APortalWall>(OutHit.GetActor()) )
		{
			if(PortalWall->GetCoordinate(OutTransform, OutHit.Location, Type, PortalScaleCoefficient))
			{
				OpenPortal(Type, PortalWall);
			}
		}
	}
}

void UPortalComponent::OpenPortal(const EPortalType Type, APortalWall* InPortalWall)
{
	if(Portals.Find(Type))
	{
		RelocatePortal(Type, InPortalWall);
	}
	else
	{
		SpawnPortal(Type, InPortalWall);
	}

	if(Portals.Num() == 2)
	{
		LinkPortal();
	}
}

void UPortalComponent::SpawnPortal(const EPortalType Type, APortalWall* InPortalWall)
{
	if(APortal* Portal = GetWorld()->SpawnActorDeferred<APortal>(PortalClass, OutTransform))
	{
		Portal->SetPortalType(Type);
		Portal->SetOwner(InPortalWall);
		Portal->SetActorScale3D(Portal->GetActorScale() * PortalScaleCoefficient);
		InPortalWall->AddLocation(Type);
		Portal->FinishSpawning(OutTransform);
		Portals.Add(Type,Portal);
	}
}

void UPortalComponent::RelocatePortal(const EPortalType Type, APortalWall* InPortalWall)
{
	// We don't use SetActorTransform because the scale of APortal can be different from 1.
	Portals[Type]->SetActorLocationAndRotation(OutTransform.GetLocation(), OutTransform.GetRotation());
	if(APortalWall* OwnerPortalWall = Cast<APortalWall>(Portals[Type]->GetOwner()))
	{
		if(OwnerPortalWall != InPortalWall)
		{
			OwnerPortalWall->RemoveLocation(Type);
			Portals[Type]->SetOwner(InPortalWall);
			InPortalWall->AddLocation(Type);
		}
		else
		{
			OwnerPortalWall->AddLocation(Type);
		}
	}
}

void UPortalComponent::LinkPortal()
{
	if(Portals[EPortalType::E_Blue]->IsLinked() == false)
	{
		Portals[EPortalType::E_Blue]->Link(Portals[EPortalType::E_Yellow]);		
	}
}

