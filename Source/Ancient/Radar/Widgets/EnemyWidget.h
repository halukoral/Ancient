// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EnemyWidget.generated.h"

UCLASS()
class ANCIENT_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateDistance(const int32 Distance) const;

private:

	int32 MaxDisplayedDistance	= 999;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DistanceText	= nullptr;
	
};
