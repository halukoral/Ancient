// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Ancient/Interfaces/Controllable.h"
#include "Components/ActorComponent.h"
#include "Enums/PortalType.h"
#include "PortalComponent.generated.h"

class APortal;
class APortalWall;

UCLASS( Blueprintable )
class ANCIENT_API UPortalComponent : public UActorComponent, public IControllable
{
	GENERATED_BODY()

public:

	UPortalComponent();

protected:

	virtual void BeginPlay() override;

public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInput(class UInputComponent* PlayerInputComponent) override;

	void Action_SpawnPortal(const EPortalType Type);

	void OpenPortal(const EPortalType Type, APortalWall* InPortalWall);

protected:

	void SpawnPortal(const EPortalType Type, APortalWall* InPortalWall);

	void RelocatePortal(const EPortalType Type, APortalWall* InPortalWall);
	
	void LinkPortal();
	
private:

	UPROPERTY(EditDefaultsOnly)
	float PortalScaleCoefficient = 1.5f;

	UPROPERTY(EditDefaultsOnly)
	float TraceDistance	= 10000.f;

	FTransform OutTransform	= FTransform::Identity;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APortal> PortalClass	= nullptr;
	
	TWeakObjectPtr<ACharacter> Character	= nullptr;

	TMap<EPortalType, APortal*, TInlineSetAllocator<2>> Portals;
};
