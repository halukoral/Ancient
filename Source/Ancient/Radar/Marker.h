// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Enums/MarkerType.h"
#include "Enums/QuestType.h"
#include "GameFramework/Actor.h"
#include "Marker.generated.h"

UCLASS()
class ANCIENT_API AMarker : public AActor
{
	GENERATED_BODY()

public:

	AMarker();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTexture2D*  GetIcon() const { return IconOnRadar; }

	EMarkerType	 GetMarkerType() const { return MarkerType; }

	EQuestType	 GetQuestType() const { return QuestType; }
	
	virtual void OnConstruction(const FTransform& Transform) override;

protected:

	UPROPERTY(EditAnywhere)
	EMarkerType	MarkerType;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "MarkerType == EMarkerType::E_Quest"))
	EQuestType	QuestType;

	UPROPERTY(EditAnywhere)
	UTexture2D*	IconOnRadar = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* BillboardComponent = nullptr;
};
