// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

class UImage;
class UTextBlock;

enum class EQuestType : uint8;

UCLASS()
class ANCIENT_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UQuestWidget(const FObjectInitializer& ObjectInitializer);

	void	SetArrowDirection(const bool IsUp) const;

	void	UpdateDistance(const int32 Distance) const;
	
	void	SetType(const EQuestType NewValue) { Type = NewValue; }

	UImage* GetArrow() const { return Arrow; }

protected:
	
	virtual void NativeConstruct() override;

private:
	
	UPROPERTY(meta = (BindWidget))
	UImage*			Arrow		 = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage*			QuestIcon	 = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock*		DistanceText = nullptr;

	EQuestType		Type;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D*		Main		 = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D*		Side		 = nullptr;

	int32			MaxDisplayedDistance = 999;
	
};
