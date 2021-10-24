// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "PortalWall.h"

APortalWall::APortalWall()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = Scene;
	
	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>("Portal Wall");
	WallMesh->SetupAttachment(RootComponent);
	WallMesh->SetCollisionProfileName("PortalWall");
}

void APortalWall::BeginPlay()
{
	Super::BeginPlay();
}

bool APortalWall::GetCoordinate(FTransform& OutTransform, const FVector& InLocation, const EPortalType InType, const float InScaleCoeff)
{	
	FVector RelativePortalLocation = GetActorTransform().InverseTransformPosition(InLocation);

	ScaleCoeff = InScaleCoeff;
	
	RelocatePortalCoordinate(RelativePortalLocation.Y, RelativePortalLocation.Z);

	PortalLocation = GetActorTransform().TransformPosition(RelativePortalLocation);
	if(!IsOverlap(PortalLocation,InType))		
	{
		PortalLocation += GetActorForwardVector() * 0.25;
		OutTransform = FTransform(GetActorRotation(),PortalLocation, OutTransform.GetScale3D());		
		return true;		
	}
	return false;
}

bool APortalWall::IsOverlap(const FVector& InLocation, const EPortalType Type)
{
	// If never created any portal before then no need to check overlapping
	if(PortalLocations.Num() == 0)
	{
		return false;
	}

	// If player wanted to spawn specific Portal, it is enough to check it with the opposite portal type.
	const EPortalType OppositeType = (Type == EPortalType::E_Blue) ? EPortalType::E_Yellow : EPortalType::E_Blue;

	if(PortalLocations.Find(OppositeType))
	{
		return IsIntersect(InLocation,PortalLocations[OppositeType]);
	}

	// If opposite type of portal don't exist, no overlap occured
	return false;
}

bool APortalWall::IsIntersect(const FVector& V1, const FVector& V2) const
{
	// Just traditional AABB vs AABB collision check
	// For more info: FBox::Intersect()
	
	const float V1_MinY = V1.Y - PortalRadiusY * ScaleCoeff;
	const float V1_MaxY = V1.Y + PortalRadiusY * ScaleCoeff;
	const float V1_MinZ = V1.Z - PortalRadiusZ * ScaleCoeff;
	const float V1_MaxZ = V1.Z + PortalRadiusZ * ScaleCoeff;

	const float V2_MinY = V2.Y - PortalRadiusY * ScaleCoeff;
	const float V2_MaxY = V2.Y + PortalRadiusY * ScaleCoeff;
	const float V2_MinZ = V2.Z - PortalRadiusZ * ScaleCoeff;
	const float V2_MaxZ = V2.Z + PortalRadiusZ * ScaleCoeff;

	if((V1_MinY > V2_MaxY) || (V2_MinY > V1_MaxY))
	{
		return false;
	}
	if((V1_MinZ > V2_MaxZ) || (V2_MinZ > V1_MaxZ))
	{
		return false;
	}
	return true;
}

void APortalWall::RelocatePortalCoordinate(float& Y, float& Z) const
{
	const FVector Size  = WallMesh->GetStaticMesh()->GetBoundingBox().GetSize();
	const float   SizeY = Size.Y * WallMesh->GetComponentScale().Y;
	const float   SizeZ = Size.Z * WallMesh->GetComponentScale().Z;
	Y = ClampPointInSurface(Y, SizeY, PortalRadiusY * ScaleCoeff);
	Z = ClampPointInSurface(Z, SizeZ, PortalRadiusZ * ScaleCoeff);
}

float APortalWall::ClampPointInSurface(const float Point, const float WallSize, const float PortalRadius)
{
	float Offset = ((WallSize / 2.f) - PortalRadius) - FMath::Abs(Point);
	Offset = FMath::Clamp(Offset, std::numeric_limits<float>::lowest(),0.f );
	if(Point >= 0.f)
	{
		Offset *= -1.f;
	}
	return Point - Offset; 
}

void APortalWall::AddLocation(const EPortalType PortalType)
{
	PortalLocations.Add(PortalType,PortalLocation);
}

void APortalWall::RemoveLocation(const EPortalType PortalType)
{
	PortalLocations.Remove(PortalType);
}