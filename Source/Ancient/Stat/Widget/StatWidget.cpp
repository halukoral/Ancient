// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "StatWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "Ancient/Stat/Structs/Stat.h"

void UStatWidget::NativeConstruct()
{
	Icon->SetBrushFromTexture(StatIcon);
	Bar->SetFillColorAndOpacity(BarColor);
}

void UStatWidget::InitializeBar(const FStat& InStat)
{
	MinValue = InStat.Min;
	CurrentValue = InStat.Current;
	MaxValue = InStat.Max;
	bInit = true;
	MaxTextBlock->SetText(FText::AsNumber(MaxValue));
	UpdateStatusBar(CurrentValue);
}

void UStatWidget::UpdateStatusBar(const int32 InValue)
{
	if (bInit)
	{
		CurrentValue = InValue;
		CurrentTextBlock->SetText(FText::AsNumber(CurrentValue));
		Bar->SetPercent((float)(CurrentValue - MinValue) / (MaxValue - MinValue));
	}
}

