// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "DirectionWidget.h"
#include "Components/TextBlock.h"

void UDirectionWidget::NativeConstruct()
{
	const FString NameStr = Direction.Name;
	const int32 NameLength = NameStr.Len();
	const FText NewText = FText::FromString(NameStr.LeftChop(NameLength - 2).ToUpper());
	
	Text->SetText(NewText);
}
