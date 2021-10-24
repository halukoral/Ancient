// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "FortniteComponent.h"

#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

#include "Data/ResourceData.h"
#include "Enums/PlayerMode.h"
#include "Placeable/Placeable.h"
#include "Placeable/PlaceableManager.h"
#include "Resource/ResourceManager.h"
#include "Widgets/FortniteWidget.h"

UFortniteComponent::UFortniteComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFortniteComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACharacter>(GetOwner());
	if(Player)
	{
		PlayerState = EPlayerMode::E_Combat;
		
		ResourceManager = Player->FindComponentByClass<UResourceManager>();
		PlaceableManager = Player->FindComponentByClass<UPlaceableManager>();
		if(ResourceManager == nullptr || PlaceableManager == nullptr)
		{
			return;
		}
		
		FortniteWidget = CreateWidget<UFortniteWidget>(GetWorld(), FortniteWidgetClass);
		if (FortniteWidget)
		{
			FortniteWidget->SetResourceManager(ResourceManager);
			FortniteWidget->SetPlaceableManager(PlaceableManager);
			FortniteWidget->GeneratePlaceableWidgets();
			FortniteWidget->GenerateResourceWidgets();
			FortniteWidget->AddToViewport();
			
			OnStateChanged();
		}
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UFortniteComponent::SightTrace, 0.05f, true);
	}
}

void UFortniteComponent::SetupInput(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Fortnite_ChangeState",	IE_Pressed, this, &UFortniteComponent::Action_ChangeState);
	PlayerInputComponent->BindAction("Fortnite_ChangeResource",	IE_Pressed, this, &UFortniteComponent::Action_ChangeResource);
	PlayerInputComponent->BindAction("Fortnite_AddResource",	IE_Pressed, this, &UFortniteComponent::Action_AddResource);
	PlayerInputComponent->BindAction("Fortnite_RemoveResource", IE_Pressed, this, &UFortniteComponent::Action_RemoveResource);
	PlayerInputComponent->BindAction("Fortnite_PlaceBuilding",	IE_Pressed, this, &UFortniteComponent::Action_Place);
	PlayerInputComponent->BindAction("Fortnite_AnyKey",			IE_Pressed, this, &UFortniteComponent::Action_AnyKey);

	PlayerInputComponent->BindAxis("Fortnite_Wheel", this, &UFortniteComponent::Action_Wheel);
}

void UFortniteComponent::SightTrace()
{
	FVector PlayerLocation;
	FRotator PlayerViewPointRotation;
	Player->GetController()->GetPlayerViewPoint(
		PlayerLocation,
		PlayerViewPointRotation
	);

	Start = Player->GetActorLocation();
	Start.Z += Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2;
	End = Start + (PlayerViewPointRotation.Vector() * 450);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	Placeable = Cast<APlaceable>(OutHit.GetActor());
	if (Placeable)
	{
		if (IsValid(PlaceableOnSight) && Placeable != PlaceableOnSight)
		{
			PlaceableOnSight->OnLeavePlayerSight();
			PlaceableOnSight = Placeable;
			PlaceableOnSight->OnEnterPlayerSight();
		}
		else if (!IsValid(PlaceableOnSight))
		{
			PlaceableOnSight = Placeable;
			PlaceableOnSight->OnEnterPlayerSight();
		}
	}
	else if (IsValid(PlaceableOnSight))
	{
		PlaceableOnSight->OnLeavePlayerSight();
		PlaceableOnSight = nullptr;
	}
}

void UFortniteComponent::Action_ChangeState()
{
	if (PlayerState == EPlayerMode::E_Combat)
	{
		PlayerState = EPlayerMode::E_Construction;
	}
	else
	{
		PlayerState = EPlayerMode::E_Combat;
	}
	OnStateChanged();
}

void UFortniteComponent::Action_ChangeResource()
{
	if(PlayerState == EPlayerMode::E_Construction)
	{
		if(ResourceManager)
		{
			ResourceManager->SelectNextWidget();
		}
	}
}

void UFortniteComponent::Action_AddResource()
{
	if(ResourceManager)
	{
		ResourceManager->AddResource(100);
	}
}

void UFortniteComponent::Action_RemoveResource()
{
	if(ResourceManager)
	{
		ResourceManager->RemoveResource(100);
	}
}

void UFortniteComponent::Action_Place()
{
	if(PlaceableManager)
	{
		PlaceableManager->Place();
	}
}

// TODO get any key properly
void UFortniteComponent::Action_AnyKey(FKey Key)
{
	// if (InKey == EKeys::AnyKey)
	// {
	// 	// Is there any key that is down
	// 	for (const TPair<FKey, FKeyState>& KeyStatePair : KeyStateMap)
	// 	{
	// 		if (KeyStatePair.Key.IsDigital() && KeyStatePair.Value.bDown)
	// 		{
	// 			return true;
	// 		}
	// 	}
	// }
	// else if (FKeyState const* const KeyState = KeyStateMap.Find(InKey))
	// {
	// 	return KeyState->bDown;
	// }
	//
	// return false;
}

void UFortniteComponent::Action_Wheel(float InValue)
{
	if(PlaceableManager && InValue != 0)
	{
		PlaceableManager->SelectPlaceable(InValue);
	}
}

void UFortniteComponent::SetPlayerState(const EPlayerMode InState)
{
	if (InState != PlayerState)
	{
		PlayerState = InState;
		OnStateChanged();
	}
}

void UFortniteComponent::OnStateChanged() const
{
	if(PlaceableManager == nullptr || FortniteWidget == nullptr)
	{
		return;
	}
	
	if (PlayerState == EPlayerMode::E_Combat)
	{
		PlaceableManager->OnStopPreviewMesh();
		FortniteWidget->SetWidgetMode(PlayerState);
	}
	else
	{
		PlaceableManager->SelectPreviewMesh(true);
		FortniteWidget->SetWidgetMode(PlayerState);
		FortniteWidget->UpdateCurrentResource();
	}
}
