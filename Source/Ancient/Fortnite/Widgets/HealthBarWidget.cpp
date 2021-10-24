// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "HealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "Ancient/Fortnite/Enums/ProgressState.h"

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

