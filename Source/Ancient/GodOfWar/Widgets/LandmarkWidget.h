// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LandmarkWidget.generated.h"

class UImage;

UCLASS()
class ANCIENT_API ULandmarkWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetLandmarkIcon(UTexture2D* NewIcon) { LandmarkIcon = NewIcon; }
	
protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage*	Icon = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D*	LandmarkIcon = nullptr;

};

