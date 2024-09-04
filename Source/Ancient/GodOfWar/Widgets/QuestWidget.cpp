// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

UQuestWidget::UQuestWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UQuestWidget::NativeConstruct()
{
	if (Type == EQuestType::E_Main)
	{
		if (Main)
		{
			QuestIcon->SetBrushFromTexture(Main, true);
		}
		Arrow->SetColorAndOpacity(FColor::FromHex("9C5B19FF"));
	}
	else
	{
		if (Side != nullptr)
		{
			QuestIcon->SetBrushFromTexture(Side, true);
		}
		Arrow->SetColorAndOpacity(FColor::FromHex("1F4FB9FF"));
	}
}

void UQuestWidget::SetArrowDirection(const bool IsUp) const
{
	if (IsUp)
	{
		Arrow->SetRenderScale(FVector2D(1.f, 1.f));
	}
	else
	{
		Arrow->SetRenderScale(FVector2D(1.f, -1.f));
	}
}

void UQuestWidget::UpdateDistance(const int32 Distance) const
{
	if (Distance > MaxDisplayedDistance)
	{
		DistanceText->SetText(FText::Format(FText::FromString(TEXT("{0}+")), FText::AsNumber(MaxDisplayedDistance)));
	}
	else
	{
		DistanceText->SetText(FText::Format(FText::FromString(TEXT("{0}m")), FText::AsNumber(FMath::Clamp<int32>(Distance, 0, MaxDisplayedDistance))));
	}
}
