// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaceableMenuWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class ANCIENT_API UPlaceableMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Update(const int32 InMaxHealth, const int32 InRequiredAmount, UTexture2D* InIcon) const;

	void SetPlaceable(const bool InValue);

private:
	bool bPlaceable = true;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*	ResourceAmountText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* ResourceIcon = nullptr;
};