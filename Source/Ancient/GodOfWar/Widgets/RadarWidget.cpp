// Fill out your copyright notice in the Description page of Project Settings.

#include "RadarWidget.h"

#include "DirectionWidget.h"
#include "EnemyWidget.h"
#include "LandmarkWidget.h"
#include "QuestWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

void URadarWidget::AddDirectionWidget(const FDirection& Info)
{
	W_Direction = CreateWidget<UDirectionWidget>(this, DirectionClass);
	if (W_Direction.Get())
	{
		W_Direction->SetDirection(Info);
		DirectionWidgets.Add(W_Direction.Get());

		FString			NameStr		{ W_Direction->GetDirection().Name };
		UOverlaySlot*	OverlaySlot { RadarOverlay->AddChildToOverlay(W_Direction.Get()) };

		OverlaySlot->SetHorizontalAlignment(HAlign_Center);
		OverlaySlot->SetVerticalAlignment(VAlign_Bottom);
	}
}

void URadarWidget::AddEnemyWidget()
{
	W_Enemy = CreateWidget<UEnemyWidget>(this, EnemyClass);
	if (W_Enemy.Get())
	{
		EnemyWidgets.Add(W_Enemy.Get());
		UOverlaySlot* OverlaySlot { RadarOverlay->AddChildToOverlay(W_Enemy.Get()) };
		OverlaySlot->SetHorizontalAlignment(HAlign_Center);
		OverlaySlot->SetVerticalAlignment(VAlign_Center);
		W_Enemy->SetPadding(FMargin(0.f, 0.f, 0.f, 7.f));
	}
}

void URadarWidget::AddQuestWidget(const EQuestType Type)
{
	W_Quest = CreateWidget<UQuestWidget>(this, QuestClass);
	if (W_Quest.Get())
	{
		W_Quest->SetType(Type);
		QuestWidgets.Add(W_Quest.Get());
		UOverlaySlot* OverlaySlot = RadarOverlay->AddChildToOverlay(W_Quest.Get());
		OverlaySlot->SetHorizontalAlignment(HAlign_Center);
		OverlaySlot->SetVerticalAlignment(VAlign_Center);
		W_Quest->SetPadding(FMargin(0.f, 5.f, 0.f, 0.f));
	}
}

void URadarWidget::AddLandmarkWidget(UTexture2D* IconTexture)
{
	W_Landmark = CreateWidget<ULandmarkWidget>(this, LandmarkClass);
	if (W_Landmark.Get())
	{
		W_Landmark->SetLandmarkIcon(IconTexture);
		LandmarkWidgets.Add(W_Landmark.Get());
		UOverlaySlot* OverlaySlot { RadarOverlay->AddChildToOverlay(W_Landmark.Get()) };
		OverlaySlot->SetHorizontalAlignment(HAlign_Center);
		OverlaySlot->SetVerticalAlignment(VAlign_Center);
	}
}

void URadarWidget::HighlightRadar(const bool IsHighlight) const
{
	if (IsHighlight)
	{
		Radar->SetColorAndOpacity(FColor::FromHex("FFA700FF"));
	}
	else
	{
		Radar->SetColorAndOpacity(FColor::FromHex("FFFFFFB3"));
	}
}

void URadarWidget::DeleteEnemyWidget(const int32 Index)
{
	if(UEnemyWidget* EnemyWidget = EnemyWidgets[Index])
	{
		EnemyWidget->RemoveFromParent();
		EnemyWidgets.Remove(EnemyWidget);		
	}
}
