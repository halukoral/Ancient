// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ancient/Fortnite/Enums/PlayerMode.h"
#include "Ancient/Fortnite/Enums/ResourceType.h"
#include "Blueprint/UserWidget.h"
#include "FortniteWidget.generated.h"

class UTextBlock;
class USizeBox;
class UImage;
class UHorizontalBox;
class UResourceManager;
class UPlaceableManager;
class UPlaceableWidget;
class UResourceWidget;

UCLASS()
class ANCIENT_API UFortniteWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetResourceManager(UResourceManager* InResourceManager);

	void SetPlaceableManager(UPlaceableManager* InPlaceableManager);

	void ShowCurrentResource(bool bShowResource) const;

	void SetWidgetMode(const EPlayerMode PlayerMode) const;

	void GenerateResourceWidgets();

	void GeneratePlaceableWidgets();

	void UpdateCurrentResource() const;

	UFUNCTION()
	void UpdateResourceAmount(const EResourceTypes InType, const int32 InAmount);
	
private:
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox*	BuildingBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* CrossHair = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentResourceAmount = nullptr;

	UPROPERTY(meta = (BindWidget))
	USizeBox* CurrentResourceBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* CurrentResourceIcon	= nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* StateIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox*	ResourceBox	= nullptr;

	TWeakObjectPtr<UResourceManager> ResourceManager = nullptr;

	TWeakObjectPtr<UPlaceableManager> PlaceableManager	= nullptr;
	
	TWeakObjectPtr<UResourceWidget> W_Resource = nullptr;
	
	TWeakObjectPtr<UPlaceableWidget> W_Placeable = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrossHair_Combat = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrossHair_Construct	= nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* IconCombat = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* IconConstruct = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UResourceWidget> ResourceWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlaceableWidget> PlaceableWidgetClass = nullptr;

	TMap<EResourceTypes, UResourceWidget*> ResourceWidgets;

};