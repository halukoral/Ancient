// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "EnemyWidget.h"

void UEnemyWidget::UpdateDistance(const int32 Distance) const
{
	if (Distance > MaxDisplayedDistance)
	{
		DistanceText->SetText(FText::Format(FText::FromString(TEXT("{0}+")), FText::AsNumber(MaxDisplayedDistance)));
	}
	else
	{
		DistanceText->SetText(FText::Format(FText::FromString(TEXT("{0}m")), FText::AsNumber(Distance)));
	}
}
