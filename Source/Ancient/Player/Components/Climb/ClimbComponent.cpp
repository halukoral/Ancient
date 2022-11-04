// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "ClimbComponent.h"

#include "Components/InputComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Ancient/Ancient.h"
#include "Ancient/Player/AncientCharacter.h"

UClimbComponent::UClimbComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LeftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Arrow"));
	LeftArrow->SetRelativeLocation(FVector(40.f, -100.f, 40.f));
	LeftArrow->SetupAttachment(this);

	LeftShoulderArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Shoulder Arrow"));
	LeftShoulderArrow->SetRelativeLocation(FVector(40.f, -10.f, 40.f));
	LeftShoulderArrow->SetupAttachment(this);

	LeftEdgeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Edge Arrow"));
	LeftEdgeArrow->SetRelativeLocation(FVector(40.f, -200.f, 40.f));
	LeftEdgeArrow->SetupAttachment(this);

	RightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Arrow"));
	RightArrow->SetRelativeLocation(FVector(40.f, 100.f, 40.f));
	RightArrow->SetupAttachment(this);

	RightShoulderArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Shoulder Arrow"));
	RightShoulderArrow->SetRelativeLocation(FVector(40.f, 10.f, 40.f));
	RightShoulderArrow->SetupAttachment(this);

	RightEdgeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Edge Arrow"));
	RightEdgeArrow->SetRelativeLocation(FVector(40.f, 200.f, 40.f));
	RightEdgeArrow->SetupAttachment(this);
}

void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AAncientCharacter>(GetOwner());
	
	IgnoredActors.Add(Character.Get());

	CollisionParams.AddIgnoredActor(Character.Get());
}

void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HeightTracer();
}

void UClimbComponent::SetupInput(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("ExitLedge", IE_Pressed, this, &UClimbComponent::Action_DropLedge);
}

void UClimbComponent::MoveForward(float Value)
{
	if (Value > 0)
	{
		// check if we have enough space above us
		if (Character->CeilCheck(Character->GetActorLocation()))
		{
			JumpUpTracer();
		}
	}
	else
	{
		bCanJumpBack = true;
	}
}

void UClimbComponent::MoveRight(float Value)
{
	if (Value > 0.f)
	{
		RightTracer();
	}
	else if (Value < 0.f)
	{
		LeftTracer();
	}
}

//////////////////////////////////////////////////////////////////////////
// Tracers

void UClimbComponent::HeightTracer()
{
	if (bHanging)
	{
		return;
	}

	// Character can grab ledge both jumping (IsFalling()) and flying (IsFlying())
	if (Character->GetCharacterMovement()->IsFalling() || Character->GetCharacterMovement()->IsFlying())
	{
		TArray<FHitResult> OutHits;

		Start = Character->GetActorLocation();
		Start.Z += 200.f;
		Start += (Character->GetActorForwardVector() * 80);

		End = Start - FVector(0.f, 0.f, 200);

		const bool bHit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			Start,
			End,
			SphereTraceRadius,
			ClimbableTraceQuery,
			false,
			IgnoredActors,
			EDrawDebugTrace::ForOneFrame,
			OutHits,
			true
		);

		if (bHit)
		{
			for (auto Hit : OutHits)
			{
				// we don't want to hold back where we jumped
				if (ClingingActor == Hit.GetActor())
				{
					return;
				}
			}

			HeightLocation = OutHits.Last().ImpactPoint;
			const FVector SocketLocation = Character->GetMesh()->GetSocketLocation(SocketName);

			if (SocketLocation.Z - HeightLocation.Z > -165.f && SocketLocation.Z - HeightLocation.Z < 0.f)
			{
				if (bClimbing == false)
				{
					// we don't need check Forward Tracer in every Tick.
					// if we found some ledge then trace Forward for ledge
					ForwardTracer(OutHits.Last().GetActor());
				}
			}
		}
		else
		{
			HeightLocation = FVector::ZeroVector;
		}
	}
}

void UClimbComponent::ForwardTracer(AActor* ClimbableActor)
{
	// Q: Why we need 2 different method for tracing?
	// A: For flying, it is difficult to detect wall location while using line trace
	// On the other hand when jumping, line trace is enough for detecting wall location

	OutHit.Reset();

	bool bHit = false;

	// Use Sphere Trace
	if (Character->GetCharacterMovement()->IsFlying())
	{
		DistToWallCoeff = 55.f;

		Start = Character->GetActorLocation();
		Start.Z += Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2;

		End = Start + (Character->GetActorForwardVector() * 200);

		bHit = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			Start,
			End,
			SphereTraceRadius,
			ClimbableTraceQuery,
			false,
			IgnoredActors,
			EDrawDebugTrace::ForOneFrame,
			OutHit,
			true
		);
	}
	else if (Character->GetCharacterMovement()->IsFalling()) // Use Line Trace
	{
		DistToWallCoeff = 50.f;

		Start = Character->GetActorLocation();
		Start.Z += Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2;

		End = Start + (Character->GetActorForwardVector() * 200);
		
		bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
	}

	if (bHit)
	{
		WallLocation = OutHit.ImpactPoint;
		WallNormal = OutHit.ImpactNormal;

		ClingingActor = ClimbableActor;

		// Can only grab if the ledge both above and in front of us
		GrabLedge();
	}
}

void UClimbComponent::LeftTracer()
{
	OutHit.Reset();

	Start = LeftArrow->GetComponentLocation();

	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		Start,
		SphereTraceRadius,
		ClimbableTraceQuery,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForOneFrame,
		OutHit,
		true
	);

	if (bHit)
	{
		bHangingLeft = true;
		HangingLeft();
	}
	else
	{
		bHangingLeft = false;
		bTryJumpOrTurn = true;
		if (LeftJumpTracer() == false)
		{
			LeftCornerTracer();
		}
	}
}

void UClimbComponent::RightTracer()
{
	OutHit.Reset();

	Start = RightArrow->GetComponentLocation();

	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		Start,
		SphereTraceRadius,
		ClimbableTraceQuery,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForOneFrame,
		OutHit,
		true
	);

	if (bHit)
	{
		bHangingRight = true;
		HangingRight();
	}
	else
	{
		bHangingRight = false;
		bTryJumpOrTurn = true;
		if (RightJumpTracer() == false)
		{
			RightCornerTracer();
		}
	}
}

bool UClimbComponent::LeftJumpTracer()
{
	OutHit.Reset();

	Start = LeftEdgeArrow->GetComponentLocation();

	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		Start,
		SphereTraceRadius,
		ClimbableTraceQuery,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForOneFrame,
		OutHit,
		true
	);

	if (bHit && bHangingLeft == false)
	{
		bCanHangingJumpLeft = true;
	}
	else
	{
		bCanHangingJumpLeft = false;
	}

	return bCanHangingJumpLeft;
}

bool UClimbComponent::RightJumpTracer()
{
	OutHit.Reset();

	Start = RightEdgeArrow->GetComponentLocation();

	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		Start,
		SphereTraceRadius,
		ClimbableTraceQuery,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForOneFrame,
		OutHit,
		true
	);

	if (bHit && bHangingRight == false)
	{
		bCanHangingJumpRight = true;
	}
	else
	{
		bCanHangingJumpRight = false;
	}

	return bCanHangingJumpRight;
}

void UClimbComponent::LeftCornerTracer()
{
	OutHit.Reset();

	Start	= LeftArrow->GetComponentLocation();
	End		= Start + (LeftArrow->GetForwardVector() * 100);

	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		End,
		SphereTraceRadius,
		ClimbableTraceQuery,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForOneFrame,
		OutHit,
		true
	);

	if (bHit)
	{
		bCanHangingCornerLeft = true;
	}
}

void UClimbComponent::RightCornerTracer()
{
	OutHit.Reset();

	Start	= RightArrow->GetComponentLocation();
	End		= Start + (RightArrow->GetForwardVector() * 100);

	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		End,
		SphereTraceRadius,
		ClimbableTraceQuery,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForOneFrame,
		OutHit,
		true
	);

	if (bHit)
	{
		bCanHangingCornerRight = true;
	}
}

void UClimbComponent::JumpUpTracer()
{
	OutHit.Reset();

	Start	= Character->GetActorLocation() + FVector(0.f, 0.f, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2);
	End		= Start + (Character->GetActorForwardVector() * 100);

	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		End,
		SphereTraceRadius,
		ClimbableTraceQuery,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForOneFrame,
		OutHit,
		true
	);

	if (bHit)
	{
		bCanJumpUp = true;
	}
}

// Tracers
//////////////////////////////////////////////////////////////////////////

void UClimbComponent::Notify_JumpUp(bool InValue)
{
	bJumpUp = InValue;

	// When animation finished this statement will execute
	if (InValue == false)
	{
		bCanJumpUp = false;

		OutHit.Reset();

		Start = Character->GetActorLocation();
		Start.Z += Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2;

		End = Start + (Character->GetActorForwardVector() * 100);

		if(GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
		{
			ClingingActor = OutHit.GetActor();
		}
	}
}

void UClimbComponent::JumpBack()
{
	if (Character->IsBusy() || Character->GetCharacterMovement()->NavAgentProps.bCanFly == false)
	{
		return;
	}

	FVector Vector = Character->GetCapsuleComponent()->GetForwardVector() * -1;
	Vector.Normalize(0.0001);
	Vector *= 400.f;
	Vector += FVector(0, 0, 400);

	if (bClimbing == false && bHanging == true)
	{
		bHanging = false;
		ClingingActor = nullptr;
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}

	FRotator Rotation = Character->GetActorRotation();
	Rotation.Yaw -= 180.f;
	Character->SetActorRotation(Rotation);

	Character->LaunchCharacter(Vector, false, false);
}

void UClimbComponent::GrabLedge()
{
	// IMPORTANT NOTE: ExecutionFunction This is the name of the function that is called 
	// and be sure to make it BlueprintCallable, otherwise it won�t be called.
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("Grabbed");
	LatentInfo.Linkage = 1;
	LatentInfo.UUID = GetNextUUID();

	// How far away from the wall our capsule
	// Don't forget to tweak DistToWallCoeff based on your model
	FVector TargetLocation = (WallNormal * DistToWallCoeff) + WallLocation;
	// Pelvis is nearly in the middle of capsule so we should 
	// extract CapsuleHalfHeight from Z value of location
	TargetLocation.Z = HeightLocation.Z - Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// multiply WallNormal with -1 because we should turn player 
	// face to wall and WallNormal direction is opposite of intended direction  
	FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(WallNormal * -1);

	// due to sphere check sometimes rotation is not normal for X axis.
	TargetRotation.Pitch = 0.f;

	UKismetSystemLibrary::MoveComponentTo(
		Character->GetCapsuleComponent(),
		TargetLocation,
		TargetRotation,
		false,
		false,
		0.1f,
		false,
		EMoveComponentAction::Type::Move,
		LatentInfo
	);
}

void UClimbComponent::Grabbed()
{
	bHanging = true;
	Character->GetCharacterMovement()->NavAgentProps.bCanJump = true;
	Character->GetCharacterMovement()->StopMovementImmediately();
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void UClimbComponent::ClimbLedge()
{
	if (bClimbing == false)
	{
		bClimbing = true;
		bHanging = false;
	}
}

void UClimbComponent::Action_DropLedge()
{
	if (bClimbing == false && bHanging == true)
	{
		bHanging = false;
		Character->GetCharacterMovement()->NavAgentProps.bCanJump = false;
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}
}

void UClimbComponent::HangingLeft()
{
	Start = LeftShoulderArrow->GetComponentLocation();
	End = Start + (LeftShoulderArrow->GetForwardVector() * 150);
	HangingMove(Start, End);
}

void UClimbComponent::HangingRight()
{
	Start = RightShoulderArrow->GetComponentLocation();
	End = Start + (RightShoulderArrow->GetForwardVector() * 150);
	HangingMove(Start, End);
}

void UClimbComponent::HangingMove(FVector InStart, FVector InEnd)
{
	OutHit.Reset();

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		FVector TargetLocation = (OutHit.ImpactNormal * DistToWallCoeff) + OutHit.Location;
		// pelvis is nearly in the middle of capsule so we should 
		// extract CapsuleHalfHeight from Z value of location
		TargetLocation.Z = Character->GetActorLocation().Z;

		FRotator TargetRotator = UKismetMathLibrary::MakeRotFromX(OutHit.ImpactNormal * -1);
		TargetRotator.Pitch = 0.f;

		Character->SetActorLocation(FMath::VInterpTo(Character->GetActorLocation(), TargetLocation, GetWorld()->GetDeltaSeconds(), 30.0f));
		Character->SetActorRotation(FMath::RInterpTo(Character->GetActorRotation(), TargetRotator, GetWorld()->GetDeltaSeconds(), 60.0f));
	}
}

void UClimbComponent::ResetMoveRight()
{
	bTryJumpOrTurn = false;

	bHangingLeft = false;
	bHangingRight = false;

	bCanHangingJumpLeft = false;
	bCanHangingJumpRight = false;

	bCanHangingCornerLeft = false;
	bCanHangingCornerRight = false;
}

void UClimbComponent::ResetMoveForward()
{
	bCanJumpUp = false;
	bCanJumpBack = false;
}

void UClimbComponent::ResetClimbActors()
{
	ClingingActor = nullptr;
}

void UClimbComponent::Notify_HangingFinished()
{
	Character->ResetJumpCounter();
	bHanging = false;
	bClimbing = false;
	ClingingActor = nullptr;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UClimbComponent::Notify_HangingJumpLeft(bool InValue)
{
	bHangingJumpLeft = InValue;

	if (InValue == false)
	{
		bCanHangingJumpLeft = InValue;

		OutHit.Reset();

		Start = Character->GetActorLocation();
		Start.Z += Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2;

		End = Start + (Character->GetActorForwardVector() * 200);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
		{
			ClingingActor = OutHit.GetActor();
		}

		// character should be able to return from where it jump
		bCanHangingJumpRight = true;
	}
}

void UClimbComponent::Notify_HangingJumpRight(bool InValue)
{
	bHangingJumpRight = InValue;

	if (InValue == false)
	{
		bCanHangingJumpRight = InValue;

		OutHit.Reset();

		Start = Character->GetActorLocation();
		Start.Z += Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2;

		End = Start + (Character->GetActorForwardVector() * 200);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
		{
			ClingingActor = OutHit.GetActor();
		}

		// character should be able to return from where it jump
		bCanHangingJumpLeft = true;
	}
}

void UClimbComponent::Notify_HangingCornerLeft(bool InValue)
{
	bHangingCornerLeft = InValue;

	if (InValue == false)
	{
		bCanHangingCornerLeft = InValue;

		OutHit.Reset();

		Start = Character->GetActorLocation();
		Start.Z += Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2;

		End = Start + (Character->GetActorForwardVector() * 200);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
		{
			Character->SetActorRotation(UKismetMathLibrary::MakeRotFromX(OutHit.Normal * -1));
		}

		// character should be able to return from where it returned
		bCanHangingCornerRight = true;
	}
}

void UClimbComponent::Notify_HangingCornerRight(bool InValue)
{
	bHangingCornerRight = InValue;

	if (InValue == false)
	{
		bCanHangingCornerRight = InValue;

		OutHit.Reset();

		Start = Character->GetActorLocation();
		Start.Z += Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2;

		End = Start + (Character->GetActorForwardVector() * 200);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
		{
			Character->SetActorRotation(UKismetMathLibrary::MakeRotFromX(OutHit.Normal * -1));
		}

		// character should be able to return from where it returned
		bCanHangingCornerLeft = true;
	}
}

void UClimbComponent::Jump()
{
	// Always check jump conditions first
	if (bCanHangingJumpRight)
	{
		Notify_HangingJumpRight(true);
	}
	else if (bCanHangingJumpLeft)
	{
		Notify_HangingJumpLeft(true);
	}
	else if (bCanHangingCornerLeft)
	{
		Notify_HangingCornerLeft(true);
	}
	else if (bCanHangingCornerRight)
	{
		Notify_HangingCornerRight(true);
	}
	else if (bCanJumpUp)
	{
		Notify_JumpUp(true);
	}
	else if (bCanJumpBack)
	{
		JumpBack();
	}
	else if (bTryJumpOrTurn == false)
	{
		// check if we have enough space over the ledge
		if (Character->CeilCheck(Character->GetActorLocation() + (Character->GetActorForwardVector() * 50)))
		{
			ClimbLedge();
		}
	}
}