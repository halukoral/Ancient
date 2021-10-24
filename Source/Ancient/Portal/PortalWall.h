// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Enums/PortalType.h"
#include "GameFramework/Actor.h"
#include "PortalWall.generated.h"


UCLASS()
class ANCIENT_API APortalWall : public AActor
{
	GENERATED_BODY()

public:
	
	APortalWall();

protected:

	virtual void	BeginPlay() override;

public:

	/* If there is enough space for portal, function return spawnable location using OutTransform */
	bool			GetCoordinate(FTransform& OutTransform, const FVector& InLocation, const EPortalType InType, const float InScaleCoeff);

	/* If Portal creation successful, we should keep the location.
	 * We can also modify the location of specific portal on the surface using this function. */
	void			AddLocation(const EPortalType PortalType);

	/* If Portal created another wall, we should remove the location from the old wall. */
	void			RemoveLocation(const EPortalType PortalType);
	
protected:

	/* We compare what is intended to be created with what already exists. */
	bool			IsOverlap(const FVector& InLocation, const EPortalType Type);

	/* AABB vs AABB */
	bool			IsIntersect(const FVector& V1, const FVector& V2) const;

	/* We want to keep the object to be created within the surface of wall mesh */
	void			RelocatePortalCoordinate(float& Y, float& Z) const;
	
	static float	ClampPointInSurface(const float Point, const float WallSize, const float PortalRadius);
	
private:

	// PortalRadiusY and PortalRadiusZ determined by SM_PortalBorder's Approx Size
	UPROPERTY(EditDefaultsOnly)
	float					PortalRadiusY	= 90.f;
	
	UPROPERTY(EditDefaultsOnly)
	float					PortalRadiusZ	= 125.f;

	float					ScaleCoeff		= 1.0f;
	
	FVector					PortalLocation	= FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly)
	USceneComponent*		Scene			= nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent*	WallMesh		= nullptr;

	UPROPERTY(VisibleAnywhere)
	TMap<EPortalType, FVector> PortalLocations;
};
