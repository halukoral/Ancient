// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Direction.h"
#include "Components/SphereComponent.h"
#include "RadarComponent.generated.h"

class AInteractable;
class URadarWidget;
class AAncientCharacter;
class UCameraComponent;
class AMarker;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANCIENT_API URadarComponent : public USphereComponent
{
	GENERATED_BODY()

public:

	URadarComponent();

	virtual void BeginPlay() override;

protected:
	
	UFUNCTION()
	void OnEnterPlayerSight(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLeavePlayerSight(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	//////////////////////////////////////////////////////////////////////////
	/// Helper Functions
	
	int32 DistanceToPlayer(const AActor* InActor) const;

	float RotToCircleDegree(const float Rotation) const;

	float GetDeltaClockwise(const float A, const float B, const bool bClockwise) const;

	void RotToTranslation(const FRotator& A, const FRotator& B, bool& bClockwise, bool& bInRadarSight, FVector2D& Translation) const;

	void OnBecomeVisible();

	void SetRadarVisibility(const bool IsVisible, const bool CanAnimate);

	void ToggleRadarVisibility(const bool CanAnimate);

	UFUNCTION(BlueprintNativeEvent)
	void VisibleAnimation();
	void VisibleAnimation_Implementation() { }

	UFUNCTION(BlueprintNativeEvent)
	void HideAnimation();
	void HideAnimation_Implementation() { }

	//////////////////////////////////////////////////////////////////////////
	/// Player's Functions
	void OnPlayerMoved();

	void OnPlayerTurned();

	//////////////////////////////////////////////////////////////////////////
	/// Landmark's Functions
	void AddLandmarks();

	void AddLandmark(AMarker* Landmark);

	void UpdateLandmarkPositions();

	//////////////////////////////////////////////////////////////////////////
	/// Direction's Functions
	void AddDirections();

	void UpdateDirectionWidgets();

	//////////////////////////////////////////////////////////////////////////
	/// Enemy's Functions

	void AddEnemy(ACharacter* Enemy);
	
	void RemoveEnemy(ACharacter* Enemy);
	
	void UpdatePositionForEnemy(ACharacter* Enemy) const;
	
	void UpdateDistanceForEnemy(ACharacter* Enemy) const;
	
	void UpdatePositionsForEnemies();
	
	void UpdateDistancesForEnemies();
	
	void OnEnemyMoved(ACharacter* Enemy) const;

	//////////////////////////////////////////////////////////////////////////
	/// Quest's Functions
 	void AddQuests();

	void AddQuest(AMarker* QuestMarker);
	
	void UpdateQuestDistance();
	
	void UpdateQuestPosition();
	
	void UpdateQuestElevation();

	//////////////////////////////////////////////////////////////////////////
	/// Secret Item's Functions
	void OnEnterSecretRange(AInteractable* Interactable);

	void OnLeaveSecretRange(AInteractable* Interactable);

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URadarWidget> RadarWidgetClass;
	
	TArray<AInteractable*> Interactables;

	TArray<FDirection> Directions;

	TArray<AMarker*> Quests;

	TArray<AMarker*> Landmarks;

	TArray<ACharacter*>	Enemies;

	TWeakObjectPtr<AAncientCharacter> Player = nullptr;

	TWeakObjectPtr<UCameraComponent> PlayerCamera = nullptr;
	
	TWeakObjectPtr<URadarWidget> RadarWidget = nullptr;

	float MaxWidgetTranslation = 320.f;

	float UnitsPerMeter = 150.f;

	float DefaultElevationRange = 150.f;

	float DelayTime = 1.f;

	bool bVisible = true;

	bool bCanChangeVisibility = true;

	bool bHideQuestMarkerWhenOutOfSight	= false;
};
