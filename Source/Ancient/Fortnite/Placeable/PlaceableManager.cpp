// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "PlaceableManager.h"
#include "Placeable.h"

#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"

#include "Ancient/Fortnite/FortniteComponent.h"
#include "Ancient/Fortnite/Data/PlaceableMeshData.h"
#include "Ancient/Fortnite/Data/PlaceableResourceData.h"
#include "Ancient/Fortnite/Data/PlaceableWidgetData.h"
#include "Ancient/Fortnite/Data/ResourceData.h"
#include "Ancient/Fortnite/Enums/PlayerMode.h"
#include "Ancient/Fortnite/Enums/ResourceType.h"
#include "Ancient/Fortnite/Resource/ResourceManager.h"
#include "Ancient/Fortnite/Widgets/PlaceableWidget.h"

UPlaceableManager::UPlaceableManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlaceableManager::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACharacter>(GetOwner());
	if(Player.Get())
	{
		PlayerCamera = Player->FindComponentByClass<UCameraComponent>();
		FortniteComponent = Player->FindComponentByClass<UFortniteComponent>();
		ResourceManager = Player->FindComponentByClass<UResourceManager>();

		if(PlayerCamera.IsValid() == false || FortniteComponent.IsValid() == false || ResourceManager.IsValid() == false)
		{
			return;
		}

		ResourceManager->OnResourceChanged.AddDynamic(this, &UPlaceableManager::OnResourceValueChanged);
		PlaceableType = EPlaceableType::E_Floor;

		if(PlaceableWidgets.Num() > 0 && FortniteComponent->GetPlayerState() == EPlayerMode::E_Construction)
		{
			SelectPreviewMesh(true);
		}
	}
}

void UPlaceableManager::SelectPreviewMesh(const bool InValue)
{
	if(UPlaceableWidget* Widget = PlaceableWidgets[PlaceableType])
	{
		Widget->ToggleWidget(InValue);
	}
	
	if(InValue)
	{
		OnStartPreviewMesh();
	}
}

void UPlaceableManager::OnStartPreviewMesh()
{
	Placeable.IsValid() ? ChangePreviewMesh() : SpawnPreviewMesh();  
}

void UPlaceableManager::OnStopPreviewMesh()
{
	SelectPreviewMesh(false);
	if (Placeable.Get())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		Placeable->SetActorHiddenInGame(true);
		Placeable->SetActorTickEnabled(false);
	}
}

void UPlaceableManager::SpawnPreviewMesh()
{
	GetPreviewMeshTransform();
	const FActorSpawnParameters SpawnInfo;
	Placeable = GetWorld()->SpawnActor<APlaceable>(PlaceableClass, Location, Rotation, SpawnInfo);
	if (Placeable.Get())
	{
		SetupPlaceable();
		Placeable->SetCanPlaceable(IsResourceEnough());
	}
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlaceableManager::SetPreviewMeshTransform, PlaceableUpdateInterval, true);
}

void UPlaceableManager::ChangePreviewMesh()
{
	if (Placeable.Get())
	{
		if(Placeable->IsHidden())
		{
			SetPreviewMeshTransform();
			if(GetWorld()->GetTimerManager().TimerExists(TimerHandle) == false)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlaceableManager::SetPreviewMeshTransform, PlaceableUpdateInterval, true);
			}
			Placeable->SetActorHiddenInGame(false);
			Placeable->SetActorTickEnabled(true);
		}
		SetupPlaceable();
		Placeable->SetCanPlaceable(IsResourceEnough());
	}
}

void UPlaceableManager::SetupPlaceable() const
{
	if (Placeable.Get() && ResourceManager.Get())
	{
		const auto ResourceData = ResourceManager->GetResourceData();
		const EResourceTypes ResourceType = ResourceManager->GetCurrentResourceType();
		const FName	ResourceRowName = GetEnumValueAsString(ResourceType);
		const FPlaceableResourceData* BuildingResourceData = DT_PlaceableResource->FindRow<FPlaceableResourceData>(ResourceRowName, "");
		
		TArray<FPlaceableMeshData*> MeshData;
		DT_PlaceableMesh->GetAllRows<FPlaceableMeshData>(FString(""), MeshData);
		
		for (const FPlaceableMeshData* Data : MeshData)
		{
			if(Data->PlaceableType == PlaceableType && Data->ResourceType == ResourceType)
			{
				Placeable->SetGhostColor(ResourceData->GhostBuildingColor);
				Placeable->SetMesh(Data->Mesh);
				Placeable->SetupWidgets(BuildingResourceData->MaxHealth, BuildingResourceData->RequiredAmount, ResourceData->Icon);
				return;
			}
		}
	}
}

void UPlaceableManager::OnResourceValueChanged(const EResourceTypes InType, const int32 InAmount)
{
	if (Placeable.Get())
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
	if (FortniteComponent.Get() && Placeable.Get() && !Placeable->IsOverlapping() && IsResourceEnough())
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
		
		if(const auto  PlaceableResourceData = DT_PlaceableResource->FindRow<FPlaceableResourceData>(RowName, ""))
		{
			ResourceManager->RemoveResource(PlaceableResourceData->RequiredAmount);

			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

			Placeable->PlaceStart(*PlaceableResourceData,*ResourceManager->GetResourceData());

			SpawnPreviewMesh();
		}
	}
}

bool UPlaceableManager::IsResourceEnough() const
{
	const FName RowName = GetEnumValueAsString( ResourceManager->GetCurrentResourceType() );
	
	if(const auto PlaceableResourceData = DT_PlaceableResource->FindRow<FPlaceableResourceData>(RowName, ""))
	{
		return ResourceManager->GetCurrentResourceAmount() >= PlaceableResourceData->RequiredAmount;			
	}
	return false;
}

void UPlaceableManager::ChangeType(const float InValue)
{
	// Select Next Building
	if(InValue > 0)
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
	else if(InValue < 0)
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

void UPlaceableManager::SetPreviewMeshTransform()
{
	GetPreviewMeshTransform();
	if(Placeable.Get())
	{
		Placeable->SetActorLocationAndRotation(Location, Rotation);
	}
}

void UPlaceableManager::GetPreviewMeshTransform()
{
	if(Player.Get() && PlayerCamera.Get())
	{
		const FVector PlayerLookAt = Player->GetActorLocation() + (PlayerCamera->GetForwardVector() * ForwardOffset);

		Location = ToGridLocation(PlayerLookAt) + LocationOffset;
		Rotation.Yaw = ToSnapRotation(PlayerCamera->GetComponentRotation().Yaw) + RotationOffset;
	}
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
