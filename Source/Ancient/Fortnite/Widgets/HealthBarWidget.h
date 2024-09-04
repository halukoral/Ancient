// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

enum class EProgressState : uint8;
class UTextBlock;
class UProgressBar;

UCLASS()
class ANCIENT_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(const int32 InMax);

	void UpdateHealth(const int32 InHealth);

	void SetProgress(const EProgressState InState);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar	= nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*	HealthText = nullptr;

	EProgressState State;
	
	int32 Max = 0;
	int32 Current = 0;

};