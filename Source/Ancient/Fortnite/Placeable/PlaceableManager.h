// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlaceableManager.generated.h"

class UCameraComponent;
class UDataTable;

class ACharacter;
class APlaceable;
class UFortniteComponent;
class UPlaceableWidget;
class UResource;
class UResourceManager;

enum class EPlaceableType : uint8;
enum class EResourceTypes : uint8;

UCLASS( Blueprintable )
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
		
	bool IsResourceEnough() const;

	void SetupPlaceable();

	void GetPreviewMeshTransform();

	void SetPreviewMeshTransform();

	void OnStartPreviewMesh();

	void SpawnPreviewMesh();

	void ChangePreviewMesh();

	void ChangeType(const float InValue);

	UFUNCTION()	
	void OnResourceValueChanged(const EResourceTypes InType, const int32 InAmount);

private:	
		
	TWeakObjectPtr<ACharacter> Player = nullptr;
	
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
	
	FTimerHandle TimerHandle;
};
