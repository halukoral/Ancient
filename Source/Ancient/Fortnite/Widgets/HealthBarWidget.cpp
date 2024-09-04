// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBarWidget.h"

#include "Ancient/Fortnite/Enums/ProgressState.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthBarWidget::Init(const int32 InMax)
{
	Max = InMax;
	SetProgress(EProgressState::E_Processing);
	UpdateHealth(0);
}

void UHealthBarWidget::UpdateHealth(const int32 InHealth)
{
	Current = FMath::Clamp(InHealth, 0, Max);
	HealthBar->SetPercent((float)Current / Max);
	HealthText->SetText(FText::Format(FText::FromString(TEXT("{0} | {1}")), FText::AsNumber(Current), FText::AsNumber(Max)));
}

void UHealthBarWidget::SetProgress(const EProgressState InState)
{
	State = InState;
	if (State == EProgressState::E_Processing)
	{
		HealthBar->SetFillColorAndOpacity(FColor::FromHex("864803FF"));
	}
	else
	{
		HealthBar->SetFillColorAndOpacity(FColor::FromHex("23A418FF"));
	}
}
