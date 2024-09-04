// Fill out your copyright notice in the Description page of Project Settings.

#include "Placeable.h"

#include "Ancient/Fortnite/Data/PlaceableResourceData.h"
#include "Ancient/Fortnite/Data/ResourceData.h"
#include "Ancient/Fortnite/Enums/ProgressState.h"
#include "Ancient/Fortnite/Widgets/HealthBarWidget.h"
#include "Ancient/Fortnite/Widgets/PlaceableMenuWidget.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

APlaceable::APlaceable()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent  = SceneComponent;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(SceneComponent);
	MeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->SetupAttachment(SceneComponent);
	BoxComponent->SetBoxExtent(FVector(90.f, 90.f, 90.f));
	BoxComponent->SetCollisionProfileName("OverlapAll");
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APlaceable::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APlaceable::OnOverlapEnd);
	
	WC_Menu = CreateDefaultSubobject<UWidgetComponent>(TEXT("Building Menu Widget Component"));
	WC_Menu->SetupAttachment(RootComponent);
	
	WC_HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar Widget Component"));
	WC_HealthBar->SetupAttachment(RootComponent);
}

void APlaceable::BeginPlay()
{
	Super::BeginPlay();

	if (WC_HealthBar && WC_Menu)
	{
		W_Menu = Cast<UPlaceableMenuWidget>(WC_Menu->GetUserWidgetObject());
		W_HealthBar = Cast<UHealthBarWidget>(WC_HealthBar->GetUserWidgetObject());
		WC_HealthBar->SetVisibility(false,false);
	}
}

void APlaceable::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		OverlappingPlaceable = Cast<APlaceable>(OtherActor);
		if (OverlappingPlaceable.IsValid() && OverlappingPlaceable->bPlaced)
		{
			OverlappingPlaceables.Add(OverlappingPlaceable.Get());
			if (bOverlapping == false)
			{
				bOverlapping = true;
				W_Menu->SetVisibility(ESlateVisibility::Hidden);
				MeshComponent->SetVisibility(false);
			}
		}
	}
}

void APlaceable::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		OverlappingPlaceable = Cast<APlaceable>(OtherActor);
		if (OverlappingPlaceable != nullptr && OverlappingPlaceable->bPlaced)
		{
			if (OverlappingPlaceables.Contains(OverlappingPlaceable.Get()))
			{
				OverlappingPlaceables.Remove(OverlappingPlaceable.Get());
				if (OverlappingPlaceables.Num() <= 0)
				{
					bOverlapping = false;
					W_Menu->SetVisibility(ESlateVisibility::Visible);
					MeshComponent->SetVisibility(true);
				}
			}
		}
	}
}

void APlaceable::OnEnterPlayerSight()
{
	bInPlayerSight = true;
	if (bPlaced)
	{
		WC_HealthBar->SetVisibility(true, false);
	}
}

void APlaceable::OnLeavePlayerSight()
{
	bInPlayerSight = false;
	if (bPlaced)
	{
		WC_HealthBar->SetVisibility(false, false);
	}
}

void APlaceable::PlaceStart(const FPlaceableResourceData& PlaceableResourceData, const FResourceData& ResourceData)
{
	bPlaced = true;

	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MeshComponent->SetMaterial(0, ResourceData.MeshMaterial);

	WC_Menu->DestroyComponent();

	PlaceableTime = PlaceableResourceData.Time;
	MaxHealth = PlaceableResourceData.MaxHealth;
	W_HealthBar->Init(MaxHealth);
	WC_HealthBar->SetVisibility(true,false);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlaceable::Placing, Interval, true);
}

void APlaceable::Placing()
{
	CurrentHealth += FGenericPlatformMath::CeilToDouble( MaxHealth / FMath::FloorToInt(PlaceableTime + Interval) );
	W_HealthBar->UpdateHealth(CurrentHealth);
		
	if (CurrentHealth == MaxHealth)
	{
		PlaceEnd();
	}
}

void APlaceable::PlaceEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	W_HealthBar->SetProgress(EProgressState::E_Finished);
}

void APlaceable::SetCanPlaceable(const bool CanPlaceable)
{
	bPlaceable = CanPlaceable;
	W_Menu->SetPlaceable(bPlaceable);
}

void APlaceable::SetMesh(UStaticMesh* InMesh)
{
	if (InMesh != nullptr && InMesh == MeshComponent->GetStaticMesh())
	{
		return;
	}
	
	if (InMesh)
	{
		MeshComponent->SetStaticMesh(InMesh);
	}
	
	if (MeshComponent->GetStaticMesh())
	{
		MID_Mesh = MeshComponent->CreateDynamicMaterialInstance(0, MeshComponent->GetMaterial(0));
		if (MID_Mesh)
		{
			MID_Mesh->SetVectorParameterValue(FName(TEXT("Color")), GhostColor);
		}
	}
}

void APlaceable::SetupWidgets(const int32 InMaxHealth,const int32 InRequiredAmount,UTexture2D* InIcon) const
{
	W_Menu->Update(InMaxHealth, InRequiredAmount, InIcon );
	WC_Menu->SetVisibility(true, false);
	WC_HealthBar->SetVisibility(false,false);
}
