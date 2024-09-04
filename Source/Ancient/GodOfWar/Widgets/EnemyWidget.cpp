// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyWidget.h"
#include "Components/TextBlock.h"

void UEnemyWidget::UpdateDistance(const int32 Distance) const
{
	if (Distance > MaxDisplayedDistance)
	{
		DistanceText->SetText(FText::Format(FText::FromString(TEXT("{0}+")), FText::AsNumber(MaxDisplayedDistance)));
	}
	else
	{
		DistanceText->SetText(FText::Format(FText::FromString(TEXT("{0}m")), FText::AsNumber(Distance)));
	}
}
