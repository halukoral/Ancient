// Fill out your copyright notice in the Description page of Project Settings.

#include "DirectionWidget.h"
#include "Components/TextBlock.h"

void UDirectionWidget::NativeConstruct()
{
	const FString NameStr = Direction.Name;
	const int32 NameLength = NameStr.Len();
	const FText NewText = FText::FromString(NameStr.LeftChop(NameLength - 2).ToUpper());
	
	Text->SetText(NewText);
}
