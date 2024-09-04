// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ancient/Fortnite/Data/PlaceableResourceData.h"
#include "Ancient/Fortnite/Data/ResourceData.h"
#include "GameFramework/Actor.h"
#include "Placeable.generated.h"

class UBoxComponent;
class UHealthBarWidget;
class UPlaceableMenuWidget;
class UWidgetComponent;

UCLASS()
class ANCIENT_API APlaceable : public AActor
{
	GENERATED_BODY()

public:
	
	APlaceable();

	virtual void BeginPlay() override;

	bool IsOverlapping() const { return bOverlapping; }

	void OnEnterPlayerSight();

	void OnLeavePlayerSight();

	void PlaceStart(const FPlaceableResourceData& PlaceableResourceData, const FResourceData& ResourceData);

	void SetCanPlaceable(const bool CanPlaceable);

	void SetMesh(UStaticMesh* InMesh);

	void SetGhostColor(const FLinearColor& Color) { GhostColor = Color; }

	void SetupWidgets(const int32 InMaxHealth,const int32 InRequiredAmount,UTexture2D* InIcon) const;
	
protected:

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void Placing();

	void PlaceEnd();
	
	bool bPlaced = false;

	bool bPlaceable	= false;

	bool bOverlapping = false;

	bool bInPlayerSight	= true;

	UPROPERTY()
	USceneComponent* SceneComponent	= nullptr; 

	UPROPERTY(EditDefaultsOnly, Category = "Building Widget")
	UWidgetComponent* WC_Menu = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Building Widget")
	UPlaceableMenuWidget* W_Menu = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Building Widget")
	UWidgetComponent* WC_HealthBar = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Building Widget")
	UHealthBarWidget* W_HealthBar = nullptr;
															
	UPROPERTY(EditDefaultsOnly, Category = "Building")	
	UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY()	
	UMaterialInstance* MI_Mesh = nullptr;

	UPROPERTY()	
	UMaterialInstanceDynamic* MID_Mesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Building")	
	UBoxComponent* BoxComponent	= nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Building")
	float PlaceableTime = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Building")	
	float Interval = 0.25f;

	int32 MaxHealth = 100;
	int32 CurrentHealth	= 0;

	TWeakObjectPtr<APlaceable> OverlappingPlaceable = nullptr;
	
	TSet<APlaceable*> OverlappingPlaceables;
	
	FLinearColor GhostColor;
	
	FTimerHandle TimerHandle;

};