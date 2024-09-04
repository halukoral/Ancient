// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceableWidget.h"

#include "Components/Image.h"
#include "Components/Spacer.h"
#include "Components/TextBlock.h"

void UPlaceableWidget::SetupWidget(const FKey InKey, UTexture2D* InTexture)
{
	HotkeyText->SetText(InKey.GetDisplayName());
	Icon->SetBrushFromTexture(InTexture, true);
	ToggleWidget(false);
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UPlaceableWidget::ToggleWidget(const bool InSelect)
{
	if (InSelect != bSelected)
	{
		bSelected = InSelect;
		if (bSelected)
		{
			Outline->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			IconSpacer->SetSize(FVector2D(1.f, 5.f));
		}
		else
		{
			Outline->SetVisibility(ESlateVisibility::Hidden);
			IconSpacer->SetSize(FVector2D(1.f, 10.f));
		}
	}
}
