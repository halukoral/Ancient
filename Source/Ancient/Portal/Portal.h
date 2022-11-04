// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Enums/PortalType.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class ACharacter;
class APortalPlayerController;

UCLASS()
class ANCIENT_API APortal : public AActor
{
	GENERATED_BODY()

public:

	APortal();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTeleportEnter(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTeleportExit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	virtual void Tick(float DeltaTime) override;

	void Link(APortal* Other);
	
	bool IsLinked() const { return LinkedPortal != nullptr; }

	void SetPortalType(const EPortalType Type) { PortalType = Type; }
	
	EPortalType GetPortalType() const { return PortalType; } 

	bool CanTeleport(const AActor* Actor) const;

	void Teleport(AActor* Actor);

	void Teleport(ACharacter* Character);
	
private:

	bool bCanEnter = true;
	
	bool bCanExit = true;

	EPortalType PortalType;

	FName ActorDefaultCollision	= FName("None");

	TWeakObjectPtr<APortalPlayerController> PC = nullptr;

	TWeakObjectPtr<ACharacter> CharacterToTeleport = nullptr;

	TWeakObjectPtr<AActor> ActorToTeleport = nullptr;

	TWeakObjectPtr<AActor> PreviewActor = nullptr;

	TWeakObjectPtr<APortal> LinkedPortal = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PortalQuality = 0.25f;

	UPROPERTY(EditDefaultsOnly)	
	USceneComponent* BackFacingScene = nullptr;

	UPROPERTY(EditDefaultsOnly)	
	USceneComponent* Scene = nullptr;

	UPROPERTY(EditDefaultsOnly)		
	USceneCaptureComponent2D* SceneCapture = nullptr;

	UPROPERTY(EditDefaultsOnly)		
	UStaticMeshComponent* Portal = nullptr;

	UPROPERTY(EditDefaultsOnly)			
	UStaticMeshComponent* PortalRing = nullptr;

	UPROPERTY(EditDefaultsOnly)			
	UMaterialInstance* MI_Yellow = nullptr;
	
	UPROPERTY(EditDefaultsOnly)			
	UMaterialInstance* MI_Blue = nullptr;

	UPROPERTY(EditDefaultsOnly)			
	UMaterialInstance* MI_RT_Yellow = nullptr;

	UPROPERTY(EditDefaultsOnly)			
	UMaterialInstance* MI_RT_Blue = nullptr;

	UPROPERTY(EditDefaultsOnly)			
	UTextureRenderTarget2D* RT_Yellow = nullptr;

	UPROPERTY(EditDefaultsOnly)			
	UTextureRenderTarget2D* RT_Blue = nullptr;
};
