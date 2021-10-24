// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "Interactable.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Interact Collision"));
	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->SetSphereRadius(500.f);
	InteractCollision->SetCollisionProfileName("OverlapAllDynamic");
	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnOverlapBegin);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnOverlapEnd);}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractable::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != this)
	{
		if(ACharacter* Character = Cast<ACharacter>(OtherActor))
		{
			if(Character->IsLocallyControlled())
			{
				
			}
		}
	}
}

void AInteractable::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor != this)
	{
		if(ACharacter* Character = Cast<ACharacter>(OtherActor))
		{
			if(Character->IsLocallyControlled())
			{
				
			}
		}
	}
}

void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

