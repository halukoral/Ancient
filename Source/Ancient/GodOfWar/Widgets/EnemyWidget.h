// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyWidget.generated.h"

class UTextBlock;

UCLASS()
class ANCIENT_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void UpdateDistance(const int32 Distance) const;

private:

	int32 MaxDisplayedDistance	= 999;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DistanceText = nullptr;
	
};
