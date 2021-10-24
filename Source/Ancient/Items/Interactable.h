// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class USphereComponent;

UCLASS()
class ANCIENT_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	
	AInteractable();

	virtual void	Tick(float DeltaTime) override;

protected:
	
	virtual void	BeginPlay() override;
	
	UFUNCTION()
	void			OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void			OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	USphereComponent* InteractCollision { nullptr };
};
