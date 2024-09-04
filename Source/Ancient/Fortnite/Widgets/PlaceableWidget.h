// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaceableWidget.generated.h"

class USpacer;
class UImage;
class UTextBlock;

UCLASS()
class ANCIENT_API UPlaceableWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void ToggleWidget(const bool InSelect);

	void SetupWidget(const FKey InKey, UTexture2D* InTexture);
	
private:
	bool bSelected = false;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock*	HotkeyText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* Icon = nullptr;

	UPROPERTY(meta = (BindWidget))
	USpacer* IconSpacer	= nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage*	Outline	= nullptr;
};
