// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UResourceWidget::SelectResource(const bool InSelected, const bool InIgnoreCheckObject)
{
	if (InSelected != bSelected || InIgnoreCheckObject)
	{
		bSelected = InSelected;
		FLinearColor Color = bSelected ? FColor::FromHex("1A2DB34C") : FColor::FromHex("0000004C");
		ResourceBorder->SetBrushColor(Color);

		Color = bSelected ? FColor::FromHex("FFFFFFCC") : FColor::FromHex("FFFFFFCC");
		AmountTextBlock->SetColorAndOpacity(Color);
	}
}

void UResourceWidget::SetupWidget(UTexture2D* InIcon, const int32 InAmount)
{
	AmountTextBlock->SetText(FText::AsNumber(InAmount));
	Icon->SetBrushFromTexture(InIcon);
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UResourceWidget::SetAmount(const int32 InAmount) const
{
	AmountTextBlock->SetText(FText::AsNumber(InAmount));
}
