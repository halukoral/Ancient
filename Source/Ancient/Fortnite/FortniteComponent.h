// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Ancient/Interfaces/Controllable.h"
#include "Components/ActorComponent.h"
#include "FortniteComponent.generated.h"

class UDataTable;
class UTexture2D;

class ACharacter;
class APlaceable;
class UFortniteWidget;
class UPlaceableManager;
class UResourceManager;

enum class EPlayerMode    : uint8;
enum class EResourceTypes : uint8;

struct FPlaceableResourceData;
struct FResourceData;

UCLASS( Blueprintable )
class ANCIENT_API UFortniteComponent : public UActorComponent, public IControllable
{
	GENERATED_BODY()

public:	

	UFortniteComponent();

	virtual void BeginPlay() override;
	
	virtual void SetupInput(class UInputComponent* PlayerInputComponent) override;

	void Action_ChangeState();

	void Action_ChangeResource();

	void Action_AddResource();

	void Action_RemoveResource();

	void Action_Place();

	void Action_AnyKey(FKey Key);

	void Action_Wheel(float InValue);
	
	/* Switch Player States between Construction and Combat	*/
	void OnStateChanged() const;

	void SightTrace();

	void SetPlayerState(const EPlayerMode InState);

	void SetPlaceableOnSight(APlaceable* InPlaceable)	{ PlaceableOnSight = InPlaceable; }

	EPlayerMode	GetPlayerState() const { return PlayerState; }

	APlaceable*	GetPlaceableOnSight() const	{ return PlaceableOnSight.Get(); }

private:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFortniteWidget> FortniteWidgetClass;

	EPlayerMode	PlayerState;

	TWeakObjectPtr<ACharacter> Player = nullptr;

	TWeakObjectPtr<APlaceable> Placeable = nullptr;

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
