// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "Marker.h"
#include "Components/BillboardComponent.h"

AMarker::AMarker()
{
	PrimaryActorTick.bCanEverTick = false;
	BillboardComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = BillboardComponent;
	SetActorScale3D(FVector(6.f, 6.f, 6.f));

#if WITH_EDITORONLY_DATA
	IconOnRadar = CreateEditorOnlyDefaultSubobject<UTexture2D>(TEXT("Icon"));
#endif 
}

void AMarker::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(IconOnRadar)
	{
		BillboardComponent->SetSprite(IconOnRadar);
	}
}
