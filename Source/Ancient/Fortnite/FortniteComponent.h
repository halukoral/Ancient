// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/PlayerMode.h"
#include "FortniteComponent.generated.h"

class UFortniteWidget;
class UResourceManager;
class UPlaceableManager;
class APlaceable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANCIENT_API UFortniteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UFortniteComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	class UInputMappingContext* IMC_OpenPortal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	class UInputAction* IA_ChangeState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	class UInputAction* IA_ChangeResource;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	class UInputAction* IA_AddResource;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	class UInputAction* IA_RemoveResource;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	class UInputAction* IA_Place;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	class UInputAction* IA_Wheel;

	/* Switch Player States between Construction and Combat	*/
	void OnStateChanged() const;

	void SightTrace();
	
	void ChangeState();
	void ChangeResource();
	void AddResource();
	void RemoveResource();
	void Place();
	void Wheel();

	void SetPlayerState(const EPlayerMode InState);

	void SetPlaceableOnSight(APlaceable* InPlaceable);

	EPlayerMode	GetPlayerState() const { return PlayerState; }

	APlaceable*	GetPlaceableOnSight() const	{ return PlaceableOnSight.Get(); }

private:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFortniteWidget> FortniteWidgetClass;

	EPlayerMode	PlayerState;

	TWeakObjectPtr<ACharacter> Player = nullptr;

	TWeakObjectPtr<APlaceable> PlaceableOnSight = nullptr;

	TWeakObjectPtr<UPlaceableManager> PlaceableManager = nullptr;

	TWeakObjectPtr<UResourceManager> ResourceManager = nullptr;

	TWeakObjectPtr<UFortniteWidget> FortniteWidget = nullptr;

	/* ------------ Traces ------------ */
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
		
	FVector Location = FVector::ZeroVector;
	FVector Start = FVector::ZeroVector;
	FVector End	= FVector::ZeroVector;
	/* ------------ ~Traces~ ------------ */

	UPROPERTY()
	FTimerHandle TimerHandle;
};
