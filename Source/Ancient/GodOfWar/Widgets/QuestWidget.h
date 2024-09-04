// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ancient/GodOfWar/Enums.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class ANCIENT_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UQuestWidget(const FObjectInitializer& ObjectInitializer);

	void SetArrowDirection(const bool IsUp) const;

	void UpdateDistance(const int32 Distance) const;
	
	void SetType(const EQuestType NewValue) { Type = NewValue; }

	UImage* GetArrow() const { return Arrow; }

protected:
	
	virtual void NativeConstruct() override;

private:
	
	EQuestType Type;

	int32 MaxDisplayedDistance = 999;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Arrow = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage*	QuestIcon = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock*	DistanceText = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D*	Main = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Side = nullptr;
};
