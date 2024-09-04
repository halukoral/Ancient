// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ancient/GodOfWar/Direction.h"
#include "Ancient/GodOfWar/Enums.h"
#include "Blueprint/UserWidget.h"
#include "RadarWidget.generated.h"

class UOverlay;
class UImage;
class UDirectionWidget;
class UEnemyWidget;
class UQuestWidget;
class ULandmarkWidget;

UCLASS()
class ANCIENT_API URadarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void DeleteEnemyWidget(const int32 Index);

	void AddEnemyWidget();

	void AddDirectionWidget(const FDirection& Info);

	void AddQuestWidget(const EQuestType Type);

	void AddLandmarkWidget(UTexture2D* IconTexture);

	void HighlightRadar(const bool IsHighlight) const;

	//////////////////////////////////////////////////////////////////////////
	/// Getter Functions
	TArray<ULandmarkWidget*> GetLandmarkWidgets() const	{ return LandmarkWidgets; }

	TArray<UDirectionWidget*> GetDirectionWidgets() const { return DirectionWidgets; }

	TArray<UEnemyWidget*> GetEnemyWidgets() const { return EnemyWidgets; }

	TArray<UQuestWidget*> GetQuestWidgets() const { return QuestWidgets; }
	//////////////////////////////////////////////////////////////////////////
	
private:
	UPROPERTY(meta = (BindWidget))
	UImage* Radar;

	UPROPERTY(meta = (BindWidget))
	UOverlay* RadarOverlay;

	UPROPERTY()
	TArray<UDirectionWidget*> DirectionWidgets;

	UPROPERTY()
	TArray<UEnemyWidget*> EnemyWidgets;

	UPROPERTY()
	TArray<UQuestWidget*> QuestWidgets;

	UPROPERTY()
	TArray<ULandmarkWidget*> LandmarkWidgets;

	UPROPERTY(EditDefaultsOnly)			
	TSubclassOf<UDirectionWidget> DirectionClass;

	UPROPERTY(EditDefaultsOnly)			
	TSubclassOf<UQuestWidget> QuestClass;

	UPROPERTY(EditDefaultsOnly)			
	TSubclassOf<ULandmarkWidget> LandmarkClass;

	UPROPERTY(EditDefaultsOnly)			
	TSubclassOf<UEnemyWidget> EnemyClass;

	TWeakObjectPtr<UQuestWidget> W_Quest = nullptr;
	TWeakObjectPtr<UEnemyWidget> W_Enemy = nullptr;
	TWeakObjectPtr<UDirectionWidget> W_Direction = nullptr;
	TWeakObjectPtr<ULandmarkWidget> W_Landmark = nullptr;
};