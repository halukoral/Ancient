// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceManager.h"

#include "Ancient/Fortnite/Widgets/ResourceWidget.h"

UResourceManager::UResourceManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// We should specify which resources should be used
	Resources.Add(EResourceTypes::E_Ice, 500);
	Resources.Add(EResourceTypes::E_Wood, 500);
	Resources.Add(EResourceTypes::E_Stone, 500);
}

void UResourceManager::BeginPlay()
{
	Super::BeginPlay();

	CurrentResource = EResourceTypes::E_Ice;

	if(ResourceWidgets.Num() > 0)
	{
		ToggleWidget(true);
	}
}

void UResourceManager::AddResource(const int32 Amount)
{
	if (Amount > 0)
	{
		Resources.Add(CurrentResource, FMath::Clamp(Amount + Resources[CurrentResource], MinValue, MaxValue));
		OnResourceChanged.Broadcast(CurrentResource, Resources[CurrentResource]);
	}
}

void UResourceManager::RemoveResource(const int32 Amount)
{
	const int32 CurrentAmount = Resources[CurrentResource]; 
	if(CurrentAmount >= Amount)
	{
		Resources.Add(CurrentResource, CurrentAmount - Amount);
		OnResourceChanged.Broadcast(CurrentResource, Resources[CurrentResource]);
	}
}

FResourceData* UResourceManager::GetResourceData() const
{
	const FName RowName	= GetEnumValueAsString(CurrentResource);
	return ResourceDataTable->FindRow<FResourceData>(RowName, "");
}

void UResourceManager::UpdateResourceWidget(const EResourceTypes Type) const
{
	OnResourceChanged.Broadcast(Type, Resources[Type]);
}

void UResourceManager::ToggleWidget(const bool InValue)
{
	if(UResourceWidget* Widget = ResourceWidgets[CurrentResource])
	{
		Widget->SelectResource(InValue,false);
	}
}

void UResourceManager::SelectNextWidget()
{
	ToggleWidget(false);
	ChangeResourceType();
	ToggleWidget(true);
	OnResourceChanged.Broadcast(CurrentResource, Resources[CurrentResource]);
}

void UResourceManager::ChangeResourceType()
{
	// Case order must be the same with DT_Resource
	switch (CurrentResource)
	{
	case EResourceTypes::E_Ice:
		CurrentResource = EResourceTypes::E_Wood;
		break;
	case EResourceTypes::E_Wood:
		CurrentResource = EResourceTypes::E_Stone;
		break;
	case EResourceTypes::E_Stone:
		CurrentResource = EResourceTypes::E_Ice;
		break;
	case EResourceTypes::E_Count: 
	default:
		break;
	}
}