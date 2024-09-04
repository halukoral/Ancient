// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ancient/Fortnite/Enums/PlaceableType.h"
#include "Components/ActorComponent.h"
#include "PlaceableManager.generated.h"


class UResourceManager;
class UFortniteComponent;
class UCameraComponent;
class APlaceable;
class AAncientCharacter;
class UPlaceableWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANCIENT_API UPlaceableManager : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPlaceableManager();
	
	virtual void BeginPlay() override;
	
	UDataTable* GetPlaceableWidgetDT() const { return DT_PlaceableWidget; }
	
	void SelectPreviewMesh(const bool InValue);

	void OnStopPreviewMesh();

	void SelectPlaceable(const float InValue);

	void SelectPlaceable(const uint8 InValue);
	
	void AddWidgets(const EPlaceableType Type, UPlaceableWidget* Widget) { PlaceableWidgets.Add(Type, Widget); }
	
	void Place();
	
protected:
	
	/** Projecting any location in the world onto a grid */
	FVector ToGridLocation(const FVector& InValue) const;

	/** Buildings are always rotated so that they are facing
	*  the same rotation as our player however it snaps 90 degrees
	*  so you are not able to reach any rotations in between */
	int32 ToSnapRotation(const float InValue) const;

	void OnPlayerMoved();

	void OnPlayerTurned();
	
	bool IsResourceEnough() const;

	bool CalculateGridTransform();

	void SetupPlaceable();

	void UpdatePreviewMeshTransform();

	void OnStartPreviewMesh();

	void SpawnPreviewMesh();

	void UpdatePreviewMesh();

	void ChangeType(const float InValue);

	UFUNCTION()	
	void OnResourceValueChanged(const EResourceTypes InType, const int32 InAmount);

private:	
		
	TWeakObjectPtr<AAncientCharacter> Player = nullptr;
	
	TWeakObjectPtr<APlaceable> Placeable = nullptr;

	TWeakObjectPtr<UCameraComponent> PlayerCamera = nullptr;
	
	TWeakObjectPtr<UFortniteComponent> FortniteComponent = nullptr;

	TWeakObjectPtr<UResourceManager> ResourceManager = nullptr;

	TMap<EPlaceableType, UPlaceableWidget*> PlaceableWidgets;

	UPROPERTY(EditDefaultsOnly, Category = "Build Data")
	UDataTable* DT_PlaceableMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Build Data")
	UDataTable*	DT_PlaceableResource = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Build Data")
	UDataTable*	DT_PlaceableWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Build Data")
	TSubclassOf<APlaceable> PlaceableClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Build Data")
	float PlaceableUpdateInterval = 0.3f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Build Data | Grid")
	int32 XGridSize = 200;

	UPROPERTY(EditDefaultsOnly, Category = "Build Data | Grid")
	int32 YGridSize	= 200;

	UPROPERTY(EditDefaultsOnly, Category = "Build Data | Grid")
	int32 ZGridSize	= 200;

	UPROPERTY(EditDefaultsOnly, Category = "Build Data | Grid")
	int32 RotationSnap = 90;

	UPROPERTY(EditDefaultsOnly, Category = "Build Data | Grid")
	int32 RotationOffset = 90;

	UPROPERTY(EditDefaultsOnly, Category = "Build Data | Grid")
	int32 ForwardOffset = 400;
	
	FRotator Rotation = FRotator::ZeroRotator;
	FVector	Location = FVector::ZeroVector;
	FVector	LocationOffset = FVector(0.f, 0.f, 0.f);
	
	EPlaceableType PlaceableType;
};
