// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

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

	TWeakObjectPtr<UDirectionWidget> W_Direction = nullptr;

	TWeakObjectPtr<UQuestWidget> W_Quest = nullptr;

	TWeakObjectPtr<ULandmarkWidget> W_Landmark = nullptr;

	TWeakObjectPtr<UEnemyWidget> W_Enemy = nullptr;
};
