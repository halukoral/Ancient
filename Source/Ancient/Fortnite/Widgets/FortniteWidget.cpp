// Fill out your copyright notice in the Description page of Project Settings.

#include "FortniteWidget.h"

#include "PlaceableWidget.h"
#include "ResourceWidget.h"
#include "Ancient/Fortnite/Data/PlaceableWidgetData.h"
#include "Ancient/Fortnite/Data/ResourceData.h"
#include "Ancient/Fortnite/Enums/PlayerMode.h"
#include "Ancient/Fortnite/Placeable/PlaceableManager.h"
#include "Ancient/Fortnite/Resource/ResourceManager.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UFortniteWidget::SetResourceManager(UResourceManager* InResourceManager)
{
	ResourceManager = InResourceManager; 
}

void UFortniteWidget::SetPlaceableManager(UPlaceableManager* InPlaceableManager)
{
	PlaceableManager = InPlaceableManager;
}

void UFortniteWidget::ShowCurrentResource(const bool bShowResource) const
{
	bShowResource ?
		CurrentResourceBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible) :
		CurrentResourceBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UFortniteWidget::SetWidgetMode(const EPlayerMode PlayerMode) const
{
	if (PlayerMode == EPlayerMode::E_Combat)
	{
		CrossHair->SetBrushFromTexture(CrossHair_Combat, true);
		StateIcon->SetBrushFromTexture(IconConstruct, true);
		BuildingBox->SetRenderOpacity(0.8f);
		BuildingBox->SetRenderScale(FVector2D(0.8f, 0.8f));
		ShowCurrentResource(false);
	}
	else
	{
		CrossHair->SetBrushFromTexture(CrossHair_Construct, true);
		StateIcon->SetBrushFromTexture(IconCombat, true);
		BuildingBox->SetRenderOpacity(1.f);
		BuildingBox->SetRenderScale(FVector2D(1.f, 1.f));
		ShowCurrentResource(true);
	}
}

void UFortniteWidget::GenerateResourceWidgets()
{
	if(ResourceManager.Get())
	{
		ResourceManager->OnResourceChanged.AddDynamic(this, &UFortniteWidget::UpdateResourceAmount);

		TArray<FResourceData*> ResourceData;
		ResourceManager->GetResourceDataTable()->GetAllRows<FResourceData>(FString(""), ResourceData);
		for (uint8 i = 0; i < ResourceData.Num(); ++i)
		{
			W_Resource = CreateWidget<UResourceWidget>(this, ResourceWidgetClass);
			if (W_Resource != nullptr)
			{
				const int32 Amount = ResourceManager->GetResourceAmount(ResourceData[i]->Type);
				W_Resource->SetupWidget(ResourceData[i]->Icon, Amount);
				CurrentResourceAmount->SetText(FText::AsNumber(Amount));

				ResourceManager->AddWidgets(ResourceData[i]->Type, W_Resource.Get());
				ResourceWidgets.Add(ResourceData[i]->Type, W_Resource.Get());
				
				const auto ResourceSlot { ResourceBox->AddChildToHorizontalBox(W_Resource.Get()) };
				if (i > 0)
				{
					ResourceSlot->SetPadding(FMargin(10.f, 0.f, 0.f, 0.f));
				}
				else
				{
					ResourceSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
				}
			}
		}
	}
}

void UFortniteWidget::GeneratePlaceableWidgets()
{
	if(PlaceableManager.Get())
	{
		TArray<FPlaceableWidgetData*> PlaceableWidgetData;
		PlaceableManager->GetPlaceableWidgetDT()->GetAllRows<FPlaceableWidgetData>(FString(""), PlaceableWidgetData);
		for (uint8 i = 0; i < PlaceableWidgetData.Num(); ++i)
		{
			W_Placeable = CreateWidget<UPlaceableWidget>(this, PlaceableWidgetClass);
			if (W_Placeable != nullptr)
			{
				W_Placeable->SetupWidget(PlaceableWidgetData[i]->Hotkey, PlaceableWidgetData[i]->Icon);
				PlaceableManager->AddWidgets(PlaceableWidgetData[i]->Type, W_Placeable.Get());

				const auto BuildingSlot = BuildingBox->AddChildToHorizontalBox(W_Placeable.Get());
				if (i > 0)
				{
					BuildingSlot->SetPadding(FMargin(10.f, 0.f, 0.f, 0.f));
				}
				else
				{
					BuildingSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
				}
			}
		}
	}
}

void UFortniteWidget::UpdateCurrentResource() const
{
	const FName RowName	= GetEnumValueAsString( ResourceManager->GetCurrentResourceType() );
	const int32 Amount	= ResourceManager->GetCurrentResourceAmount();

	if(const FResourceData* Data = ResourceManager->GetResourceDataTable()->FindRow<FResourceData>(FName(RowName), ""))
	{
		CurrentResourceIcon->SetBrushFromTexture(Data->Icon);
	}
	CurrentResourceAmount->SetText(FText::AsNumber(Amount));
}

void UFortniteWidget::UpdateResourceAmount(const EResourceTypes InType, const int32 InAmount) 
{
	UpdateCurrentResource();

	if(const UResourceWidget* Widget = ResourceWidgets[InType] )
	{
		Widget->SetAmount(InAmount);
	}
	
	if(InType == ResourceManager->GetCurrentResourceType())
	{
		CurrentResourceAmount->SetText(FText::AsNumber(InAmount));
	}
}