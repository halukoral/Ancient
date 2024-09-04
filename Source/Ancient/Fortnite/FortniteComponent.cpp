// Fill out your copyright notice in the Description page of Project Settings.

#include "FortniteComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
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
	if (Player.IsValid())
	{
		// Set up action bindings
		if (const APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
				Subsystem->AddMappingContext(IMC_OpenPortal, 1);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				EnhancedInputComponent->BindAction(IA_ChangeState, ETriggerEvent::Triggered, this, &UFortniteComponent::ChangeState);
				EnhancedInputComponent->BindAction(IA_ChangeResource, ETriggerEvent::Triggered, this, &UFortniteComponent::ChangeResource);
				EnhancedInputComponent->BindAction(IA_AddResource, ETriggerEvent::Triggered, this, &UFortniteComponent::AddResource);
				EnhancedInputComponent->BindAction(IA_RemoveResource, ETriggerEvent::Triggered, this, &UFortniteComponent::RemoveResource);
				EnhancedInputComponent->BindAction(IA_Place, ETriggerEvent::Triggered, this, &UFortniteComponent::Place);
				EnhancedInputComponent->BindAction(IA_Wheel, ETriggerEvent::Triggered, this, &UFortniteComponent::Wheel);
			}
		}

		PlayerState = EPlayerMode::E_Combat;
		
		ResourceManager = Player->FindComponentByClass<UResourceManager>();
		PlaceableManager = Player->FindComponentByClass<UPlaceableManager>();
		if (ResourceManager == nullptr || PlaceableManager == nullptr)
		{
			return;
		}
		
		FortniteWidget = CreateWidget<UFortniteWidget>(GetWorld(), FortniteWidgetClass);
		if (FortniteWidget.IsValid())
		{
			FortniteWidget->SetResourceManager(ResourceManager.Get());
			FortniteWidget->SetPlaceableManager(PlaceableManager.Get());
			FortniteWidget->GeneratePlaceableWidgets();
			FortniteWidget->GenerateResourceWidgets();
			FortniteWidget->AddToViewport();
			
			OnStateChanged();
		}
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UFortniteComponent::SightTrace, 0.05f, true);
	}
}

void UFortniteComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Player == nullptr)
	{
		return;
	}

	if (const APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(IMC_OpenPortal);
		}
	}
	
	Super::EndPlay(EndPlayReason);
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

	if (APlaceable* Placeable = Cast<APlaceable>(OutHit.GetActor()))
	{
		if (PlaceableOnSight.IsValid() && Placeable != PlaceableOnSight.Get())
		{
			PlaceableOnSight->OnLeavePlayerSight();
			PlaceableOnSight = Placeable;
			PlaceableOnSight->OnEnterPlayerSight();
		}
		else if (PlaceableOnSight.IsValid() == false)
		{
			PlaceableOnSight = Placeable;
			PlaceableOnSight->OnEnterPlayerSight();
		}
	}
	else if (PlaceableOnSight.IsValid())
	{
		PlaceableOnSight->OnLeavePlayerSight();
		PlaceableOnSight.Reset();
	}
}

void UFortniteComponent::ChangeState()
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

void UFortniteComponent::ChangeResource()
{
	if (PlayerState == EPlayerMode::E_Construction)
	{
		if (ResourceManager.IsValid())
		{
			ResourceManager->SelectNextWidget();
		}
	}
}

void UFortniteComponent::AddResource()
{
	if (ResourceManager.IsValid())
	{
		ResourceManager->AddResource(100);
	}
}

void UFortniteComponent::RemoveResource()
{
	if (ResourceManager.IsValid())
	{
		ResourceManager->RemoveResource(100);
	}
}

void UFortniteComponent::Place()
{
	if (PlaceableManager.IsValid())
	{
		PlaceableManager->Place();
	}
}

void UFortniteComponent::Wheel()
{
	if (PlaceableManager.IsValid())
	{
		static int x = 1;
		if (x > 4)
		{
			x %= 4;
		}
		if (x == 0)
		{
			x = 1;
		}
		PlaceableManager->SelectPlaceable(1.f);
		++x;
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

void UFortniteComponent::SetPlaceableOnSight(APlaceable* InPlaceable)
{
	PlaceableOnSight = InPlaceable;
}

void UFortniteComponent::OnStateChanged() const
{
	if (PlaceableManager.IsValid() && FortniteWidget.IsValid())
	{
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
}
