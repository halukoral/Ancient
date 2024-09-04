// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceableMenuWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UPlaceableMenuWidget::Update(const int32 InMaxHealth, const int32 InRequiredAmount, UTexture2D* InIcon) const
{
	HealthText->SetText(FText::Format(FText::FromString(TEXT("{0} HP")), InMaxHealth));
	ResourceAmountText->SetText(FText::AsNumber(InRequiredAmount));
	ResourceIcon->SetBrushFromTexture(InIcon, true);
}

void UPlaceableMenuWidget::SetPlaceable(const bool InValue)
{
	if (bPlaceable != InValue)
	{
		bPlaceable = InValue;
		if (bPlaceable)
		{
			ResourceIcon->SetColorAndOpacity(FColor::FromHex("FFFFFFFF"));
			ResourceAmountText->SetColorAndOpacity(FSlateColor(FColor::FromHex("FFFFFFE6")));
		}
		else
		{
			ResourceIcon->SetColorAndOpacity(FColor::FromHex("FF393FFF"));
			ResourceAmountText->SetColorAndOpacity(FSlateColor(FColor::FromHex("FF393FFF")));
		}
	}
}
