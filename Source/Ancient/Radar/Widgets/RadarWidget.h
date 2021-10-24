// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadarWidget.generated.h"

class UDirectionWidget;
class UEnemyWidget;
class UImage;
class ULandmarkWidget;
class UOverlay;
class UQuestWidget;

enum class EQuestType : uint8;

struct FDirection;

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
	TArray<ULandmarkWidget*>	GetLandmarkWidgets() const	{ return LandmarkWidgets; }

	TArray<UDirectionWidget*>	GetDirectionWidgets() const { return DirectionWidgets; }

	TArray<UEnemyWidget*>		GetEnemyWidgets() const		{ return EnemyWidgets; }

	TArray<UQuestWidget*>		GetQuestWidgets() const		{ return QuestWidgets; }
	//////////////////////////////////////////////////////////////////////////
	
private:

	UPROPERTY(meta = (BindWidget))
	UImage*							Radar;

	UPROPERTY(meta = (BindWidget))
	UOverlay*						RadarOverlay;

	TArray<UDirectionWidget*>		DirectionWidgets;

	TArray<UEnemyWidget*>			EnemyWidgets;

	TArray<UQuestWidget*>			QuestWidgets;

	TArray<ULandmarkWidget*>		LandmarkWidgets;

	UDirectionWidget*				W_Direction = nullptr;
			
	UQuestWidget*					W_Quest		= nullptr;
			
	ULandmarkWidget*				W_Landmark	= nullptr;
			
	UEnemyWidget*					W_Enemy		= nullptr;

	UPROPERTY(EditDefaultsOnly)			
	TSubclassOf<UDirectionWidget>	DirectionClass;

	UPROPERTY(EditDefaultsOnly)			
	TSubclassOf<UQuestWidget>		QuestClass;

	UPROPERTY(EditDefaultsOnly)			
	TSubclassOf<ULandmarkWidget>	LandmarkClass;

	UPROPERTY(EditDefaultsOnly)			
	TSubclassOf<UEnemyWidget>		EnemyClass;
};
