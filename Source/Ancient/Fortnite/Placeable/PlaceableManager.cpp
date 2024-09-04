// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceableManager.h"

#include "Placeable.h"
#include "Ancient/AncientCharacter.h"
#include "Ancient/Fortnite/FortniteComponent.h"
#include "Ancient/Fortnite/Data/PlaceableMeshData.h"
#include "Ancient/Fortnite/Data/PlaceableResourceData.h"
#include "Ancient/Fortnite/Data/ResourceData.h"
#include "Ancient/Fortnite/Enums/ResourceType.h"
#include "Ancient/Fortnite/Resource/ResourceManager.h"
#include "Ancient/Fortnite/Widgets/PlaceableWidget.h"
#include "Camera/CameraComponent.h"

struct FPlaceableMeshData;

UPlaceableManager::UPlaceableManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlaceableManager::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AAncientCharacter>(GetOwner());
	if (Player.IsValid())
	{
		PlayerCamera = Player->FindComponentByClass<UCameraComponent>();
		FortniteComponent = Player->FindComponentByClass<UFortniteComponent>();
		ResourceManager = Player->FindComponentByClass<UResourceManager>();

		if (PlayerCamera.IsValid() == false || FortniteComponent.IsValid() == false || ResourceManager.IsValid() == false)
		{
			return;
		}
		
		Player->OnPlayerMoved.AddUObject(this, &UPlaceableManager::OnPlayerMoved);
		Player->OnPlayerTurned.AddUObject(this, &UPlaceableManager::OnPlayerTurned);

		ResourceManager->OnResourceChanged.AddDynamic(this, &UPlaceableManager::OnResourceValueChanged);

		PlaceableType = EPlaceableType::E_Floor;

		if (PlaceableWidgets.Num() > 0 && FortniteComponent->GetPlayerState() == EPlayerMode::E_Construction)
		{
			SelectPreviewMesh(true);
		}
	}
}

void UPlaceableManager::OnPlayerMoved()
{
	if (FortniteComponent->GetPlayerState() == EPlayerMode::E_Construction)
	{
		if (FortniteComponent->GetPlaceableOnSight() == nullptr)
		{
			if (Placeable.IsValid())
			{
				UpdatePreviewMesh();
			}
			else
			{
				SpawnPreviewMesh();
			}
		}
	}
}

void UPlaceableManager::OnPlayerTurned()
{
	if (FortniteComponent->GetPlayerState() == EPlayerMode::E_Construction)
	{
		if (FortniteComponent->GetPlaceableOnSight() == nullptr)
		{
			if (Placeable.IsValid())
			{
				UpdatePreviewMesh();
			}
			else
			{
				SpawnPreviewMesh();
			}
		}
	}
}

void UPlaceableManager::SelectPreviewMesh(const bool InValue)
{
	if (UPlaceableWidget* Widget = PlaceableWidgets[PlaceableType])
	{
		Widget->ToggleWidget(InValue);
	}
	
	if (InValue)
	{
		OnStartPreviewMesh();
	}
}

void UPlaceableManager::OnStartPreviewMesh()
{
	Placeable.IsValid() ? UpdatePreviewMesh() : SpawnPreviewMesh();  
}

void UPlaceableManager::OnStopPreviewMesh()
{
	SelectPreviewMesh(false);
	if (Placeable.IsValid())
	{
		Placeable->SetActorHiddenInGame(true);
	}
}

void UPlaceableManager::SpawnPreviewMesh()
{
	CalculateGridTransform();
	
	const FActorSpawnParameters SpawnInfo;
	Placeable = GetWorld()->SpawnActor<APlaceable>(PlaceableClass, Location, Rotation, SpawnInfo);
	if (Placeable.IsValid())
	{
		SetupPlaceable();
		if (Placeable->IsOverlapping())
		{
			OnStopPreviewMesh();
		}
		else
		{
			Placeable->SetCanPlaceable(IsResourceEnough());
		}
	}
}

void UPlaceableManager::UpdatePreviewMesh()
{
	if (Placeable.IsValid())
	{
		UpdatePreviewMeshTransform();
				
		SetupPlaceable();
		if (Placeable->IsOverlapping())
		{
			OnStopPreviewMesh();
		}
		else
		{
			if (Placeable->IsHidden())
			{
				Placeable->SetActorHiddenInGame(false);
			}
			
			Placeable->SetCanPlaceable(IsResourceEnough());
		}
	}
}

void UPlaceableManager::SetupPlaceable()
{
	if (Placeable.IsValid() && ResourceManager.IsValid())
	{
		const auto ResourceData = ResourceManager->GetResourceData();
		const EResourceTypes ResourceType = ResourceManager->GetCurrentResourceType();
		const FName	ResourceRowName = GetEnumValueAsString(ResourceType);
		const FPlaceableResourceData* BuildingResourceData = DT_PlaceableResource->FindRow<FPlaceableResourceData>(ResourceRowName, "");
		
		TArray<FPlaceableMeshData*> MeshData;
		DT_PlaceableMesh->GetAllRows<FPlaceableMeshData>(FString(""), MeshData);
		
		for (const FPlaceableMeshData* Data : MeshData)
		{
			if (Data->PlaceableType == PlaceableType && Data->ResourceType == ResourceType)
			{
				Placeable->SetGhostColor(ResourceData->GhostBuildingColor);
				Placeable->SetMesh(Data->Mesh);
				Placeable->SetupWidgets(BuildingResourceData->MaxHealth, BuildingResourceData->RequiredAmount, ResourceData->Icon);
				LocationOffset = Data->LocationOffset;
				break;
			}
		}
	}
}

void UPlaceableManager::OnResourceValueChanged(const EResourceTypes InType, const int32 InAmount)
{
	if (Placeable.IsValid())
	{
		SetupPlaceable();
		Placeable->SetCanPlaceable(IsResourceEnough());
	}
}

void UPlaceableManager::SelectPlaceable(const float InValue)
{
	if (FortniteComponent->GetPlayerState() == EPlayerMode::E_Combat)
	{
		FortniteComponent->SetPlayerState(EPlayerMode::E_Construction);
	}
	else
	{
		SelectPreviewMesh(false);
		ChangeType(InValue);
		SelectPreviewMesh(true);
	}
}

void UPlaceableManager::SelectPlaceable(const uint8 InValue)
{
	if (FortniteComponent->GetPlayerState() == EPlayerMode::E_Combat)
	{
		FortniteComponent->SetPlayerState(EPlayerMode::E_Construction);
	}
	else
	{
		SelectPreviewMesh(false);
		
		switch (InValue)
		{
		case 1:
			PlaceableType = EPlaceableType::E_Wall;
			break;
		case 2:
			PlaceableType = EPlaceableType::E_Stair;
			break;
		case 3:
			PlaceableType = EPlaceableType::E_Floor;
			break;
		default:
			break;
		}
		
		SelectPreviewMesh(true);
	}
}

void UPlaceableManager::Place()
{
	if (FortniteComponent.IsValid() && Placeable.IsValid() && Placeable->IsOverlapping() == false && IsResourceEnough())
	{
		if (FortniteComponent->GetPlaceableOnSight())
		{
			if (Placeable != FortniteComponent->GetPlaceableOnSight())
			{
				FortniteComponent->GetPlaceableOnSight()->OnLeavePlayerSight();
			}
		}
		FortniteComponent->SetPlaceableOnSight(Placeable.Get());
		
		const FName RowName = GetEnumValueAsString( ResourceManager->GetCurrentResourceType() );
		if (const auto  PlaceableResourceData = DT_PlaceableResource->FindRow<FPlaceableResourceData>(RowName, ""))
		{
			ResourceManager->RemoveResource(PlaceableResourceData->RequiredAmount);
			Placeable->PlaceStart(*PlaceableResourceData,*ResourceManager->GetResourceData());
			Placeable.Reset();
		}
	}
}

bool UPlaceableManager::IsResourceEnough() const
{
	const FName RowName = GetEnumValueAsString( ResourceManager->GetCurrentResourceType() );
	if (const auto PlaceableResourceData = DT_PlaceableResource->FindRow<FPlaceableResourceData>(RowName, ""))
	{
		return ResourceManager->GetCurrentResourceAmount() >= PlaceableResourceData->RequiredAmount;			
	}
	return false;
}

void UPlaceableManager::ChangeType(const float InValue)
{
	// Select Next Building
	if (InValue > 0)
	{
		switch (PlaceableType)
		{
		case EPlaceableType::E_Floor:
			PlaceableType = EPlaceableType::E_Stair;
			break;
		case EPlaceableType::E_Stair:
			PlaceableType = EPlaceableType::E_Wall;
			break;
		case EPlaceableType::E_Wall:
			PlaceableType = EPlaceableType::E_Floor;
			break;
		}
	}
	// Select Previous Building
	else if (InValue < 0)
	{
		switch (PlaceableType)
		{
		case EPlaceableType::E_Floor:
			PlaceableType = EPlaceableType::E_Wall;
			break;
		case EPlaceableType::E_Stair:
			PlaceableType = EPlaceableType::E_Floor;
			break;
		case EPlaceableType::E_Wall:
			PlaceableType = EPlaceableType::E_Stair;
			break;
		}
	}
}

void UPlaceableManager::UpdatePreviewMeshTransform()
{
	if (Placeable.IsValid())
	{
		if (CalculateGridTransform())
		{
			Placeable->SetActorLocationAndRotation(Location, Rotation);			
		}
	}
}

bool UPlaceableManager::CalculateGridTransform()
{
	if (Player.IsValid() && PlayerCamera.IsValid())
	{
		const FVector OldLocation = Location;
		const FRotator OldRotation = Rotation;
		
		const FVector PlayerLookAt = Player->GetActorLocation() + (PlayerCamera->GetForwardVector() * ForwardOffset);
		Location = ToGridLocation(PlayerLookAt) + LocationOffset;
		Rotation.Yaw = ToSnapRotation(PlayerCamera->GetComponentRotation().Yaw) + RotationOffset;

		return OldLocation != Location || OldRotation != Rotation;
	}
	return false;
}

FVector UPlaceableManager::ToGridLocation(const FVector& InValue) const
{
	return FVector(
		FMath::RoundToInt(InValue.X / XGridSize) * XGridSize,
		FMath::RoundToInt(InValue.Y / YGridSize) * YGridSize,
		FMath::RoundToInt(InValue.Z / ZGridSize) * ZGridSize
	);
}

int32 UPlaceableManager::ToSnapRotation(const float InValue) const
{
	return FMath::RoundToInt(InValue / RotationSnap) * RotationSnap;
}
