// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceWidget.generated.h"

class UBorder;
class UTextBlock;
class UImage;

UCLASS()
class ANCIENT_API UResourceWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SelectResource(const bool InSelected, const bool InIgnoreCheckObject);

	void SetupWidget(UTexture2D* InIcon, const int32 InAmount);

	void SetAmount(const int32 InAmount) const;
		
private:
	bool bSelected = false;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock*	AmountTextBlock	= nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* Icon = nullptr;

	UPROPERTY(meta = (BindWidget))
	UBorder* ResourceBorder	= nullptr;

};