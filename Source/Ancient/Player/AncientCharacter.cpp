// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#include "AncientCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Ancient/Interfaces/Controllable.h"
#include "Components/Climb/ClimbComponent.h"

AAncientCharacter::AAncientCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanFly = true;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->BrakingDecelerationFlying = 1500.0f;

	GetCharacterMovement()->MaxFlySpeed = FlySpeed;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Create a capsule collision for interacting
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionCapsule"));
	CapsuleCollision->SetupAttachment(RootComponent);
	CapsuleCollision->SetCapsuleHalfHeight(75.0f);
	CapsuleCollision->SetCapsuleRadius(50.0f);
	CapsuleCollision->SetRelativeLocation(FVector(40.f, 0.f, 20.f));
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = TargetArmLength_Walk; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ClimbComponent = CreateDefaultSubobject<UClimbComponent>(TEXT("Climb Component"));
	ClimbComponent->SetupAttachment(RootComponent);

}

void AAncientCharacter::BeginPlay()
{
	Super::BeginPlay();

	IgnoredActors.Add(this);

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AAncientCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFlying())
	{
		if (bRunning)
		{
			if (FMath::Abs(CameraBoom->TargetArmLength - TargetArmLength_FlyRough) > 0.01)
			{
				CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetArmLength_FlyRough, DeltaTime, 4);
			}
		}
		else
		{
			if (FMath::Abs(CameraBoom->TargetArmLength - TargetArmLength_Fly) > 0.01)
			{
				CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetArmLength_Fly, DeltaTime, 4);
			}
		}
	}
	else
	{
		if (bRunning)
		{
			if (FMath::Abs(CameraBoom->TargetArmLength - TargetArmLength_Run) > 0.01)
			{
				CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetArmLength_Run, DeltaTime, 4);
			}
		}
		else
		{
			if (FMath::Abs(CameraBoom->TargetArmLength - TargetArmLength_Walk) > 0.01)
			{
				CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetArmLength_Walk, DeltaTime, 4);
			}
		}
	}

	FloorCheck();
}

void AAncientCharacter::FloorCheck()
{
	if (ClimbComponent && (ClimbComponent->IsHanging() == true || ClimbComponent->IsClimbing() == true))
	{
		return;
	}

	if (GetCharacterMovement()->IsFlying())
	{
		OutHit.Reset();

		Start = GetActorLocation();
		End = Start - FVector(0.f, 0.f, 100);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
		{
			if (OutHit.GetActor()->GetComponentsCollisionResponseToChannel(ECollisionChannel::ECC_Pawn) == ECollisionResponse::ECR_Block)
			{
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
			}
		}
	}
}

bool AAncientCharacter::CeilCheck(FVector Loc)
{
	OutHit.Reset();

	Start = Loc;
	End = Loc + FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2);

	if (ClimbComponent)
	{
		IgnoredActors.Add(ClimbComponent->GetClingingActor());
	}

	const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		Start,
		End,
		SphereTraceRadius / 2,
		ObjectTypes,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForOneFrame,
		OutHit,
		true
	);

	if (ClimbComponent)
	{
		IgnoredActors.Remove(ClimbComponent->GetClingingActor());
	}

	if (bHit)
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAncientCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAncientCharacter::Action_Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	DECLARE_DELEGATE_OneParam(FCustomInputDelegate, const bool);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("Crouch", IE_Pressed, this, &ACharacter::Crouch, false);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("Crouch", IE_Released, this, &ACharacter::UnCrouch, false);

	PlayerInputComponent->BindAction("Sneak", IE_Pressed, this, &AAncientCharacter::Action_Sneak);
	PlayerInputComponent->BindAction("Sneak", IE_Released, this, &AAncientCharacter::Action_StopSneak);

	PlayerInputComponent->BindAction("Celebrate", IE_Pressed, this, &AAncientCharacter::Action_Celebrate);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AAncientCharacter::Action_Sprint);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("Sprint", IE_Released, this, &AAncientCharacter::Action_StopSprint, false);

	PlayerInputComponent->BindAction("CrouchToggle", IE_Pressed, this, &AAncientCharacter::Action_CrouchToggle);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AAncientCharacter::Action_Interact);

	PlayerInputComponent->BindAction("InteractLoop", IE_Pressed, this, &AAncientCharacter::Action_InteractLoopStart);
	PlayerInputComponent->BindAction("InteractLoop", IE_Released, this, &AAncientCharacter::Action_InteractLoopEnd);

	PlayerInputComponent->BindAxis("Ascend", this, &AAncientCharacter::Action_Ascend);
	PlayerInputComponent->BindAxis("Descend", this, &AAncientCharacter::Action_Descend);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAncientCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAncientCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turn rate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AAncientCharacter::Turn);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAncientCharacter::TurnAtRate);
	
	PlayerInputComponent->BindAxis("LookUp", this, &AAncientCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAncientCharacter::LookUpAtRate);

	for(const auto Component : GetComponents())
	{
		if (IControllable* Controllable = Cast<IControllable>(Component))
		{
			Controllable->SetupInput(PlayerInputComponent);
		}
	}
}

void AAncientCharacter::Turn(float Value)
{
	if (Value != 0)
	{
		OnPlayerTurned.Broadcast();
	}
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());	
}

void AAncientCharacter::TurnAtRate(float Rate)
{
	if (Rate != 0.f)
	{
		OnPlayerTurned.Broadcast();
	}
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());	
}

void AAncientCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAncientCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAncientCharacter::MoveForward(float Value)
{
	if (ClimbComponent)
	{
		ClimbComponent->ResetMoveForward();
	}
	
	if (Controller != nullptr && Value != 0.0f)
	{
		OnPlayerMoved.Broadcast();

		if (ClimbComponent && ClimbComponent->IsHanging())
		{
			ClimbComponent->MoveForward(Value);
		}
		else if (GetCharacterMovement()->IsFlying())
		{
			const FVector Direction = FollowCamera->GetForwardVector();
			
			//float AimAtAngle = ((acosf(FVector::DotProduct(AscendDescendDirection, Direction))) * (180 / 3.1415926));

			if (GetCharacterMovement()->Velocity.Size() > 0)
			{
				GetCharacterMovement()->bUseControllerDesiredRotation = true;
			}
			else
			{
				GetCharacterMovement()->bUseControllerDesiredRotation = false;
			}
			
			AddMovementInput(Direction, Value);
		}
		else if (GetCharacterMovement()->IsWalking())
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}	
	}
}

void AAncientCharacter::MoveRight(float Value)
{
	if (ClimbComponent)
	{
		if (ClimbComponent->IsJumpBack())
		{
			return;
		}

		ClimbComponent->ResetMoveRight();
	}
	
	if (Controller != nullptr && Value != 0.0f)
	{
		OnPlayerMoved.Broadcast();
		
		if (ClimbComponent && ClimbComponent->IsHanging())
		{
			ClimbComponent->MoveRight(Value);
		}
		else if (GetCharacterMovement()->IsFlying() || GetCharacterMovement()->IsWalking())
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
}

// Input
//////////////////////////////////////////////////////////////////////////

void AAncientCharacter::Action_Jump()
{
	if (ClimbComponent && ClimbComponent->IsClimbing() == true)
	{
		return;
	}

	if (IsBusy() || GetCharacterMovement()->NavAgentProps.bCanJump == false)
	{
		return;
	}

	if (ClimbComponent && ClimbComponent->IsHanging())
	{
		ClimbComponent->Jump();
	}
	else
	{
		// check if we have enough space over the character
		if (CeilCheck(GetActorLocation()) == false)
		{
			return;
		}

		switch (JumpCounter)
		{
		case 0:
			++JumpCounter;
			Super::Jump();
			break;
		case 1:
			Fly();
			break;
		case 2:
			if (GetCharacterMovement()->IsFlying())
			{
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
			}
			break;
		default:
			break;
		}
	}
}

void AAncientCharacter::Notify_JumpFinished()
{
	JumpCounter = 0;
	GetCharacterMovement()->NavAgentProps.bCanJump = true;
	
	if (ClimbComponent)
	{
		ClimbComponent->ResetClimbActors();
	}
}

void AAncientCharacter::Fly()
{
	if (IsBusy() || GetCharacterMovement()->NavAgentProps.bCanFly == false)
	{
		return;
	}

	FVector Vector = GetCapsuleComponent()->GetForwardVector();
	Vector.Normalize(0.0001);
	Vector *= 400.f;
	Vector += FVector(0, 0, 400);

	LaunchCharacter(Vector, false, false);

	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AAncientCharacter::SetFly, 0.05f, false);
}

void AAncientCharacter::SetFly()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	++JumpCounter;
}

void AAncientCharacter::Crouch(bool bClientSimulation /*= false*/)
{
	if (ClimbComponent && ClimbComponent->IsHanging() == true)
	{
		return;
	}

	if (GetCharacterMovement()->IsWalking())
	{
		if (bRunning)
		{
			Action_StopSprint(true);
		}

		GetCharacterMovement()->MaxWalkSpeed = CrouchWalkSpeed;
		GetCharacterMovement()->NavAgentProps.bCanJump = false;
		GetCharacterMovement()->NavAgentProps.bCanFly = false;
		Super::Crouch();
	}
}

void AAncientCharacter::UnCrouch(bool bClientSimulation /*= false*/)
{
	if (ClimbComponent && ClimbComponent->IsHanging() == true)
	{
		return;
	}

	if (GetCharacterMovement()->IsWalking())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		GetCharacterMovement()->NavAgentProps.bCanJump = true;
		GetCharacterMovement()->NavAgentProps.bCanFly = true;
		Super::UnCrouch();
		
		if (bRunningKeyPressed)
		{
			Action_Sprint();
		}
	}
}

bool AAncientCharacter::IsBusy() const
{
	return bInteracting || bInteractingLoop || bInteractLoopAnim;
}

void AAncientCharacter::Action_Celebrate()
{
	bCelebrating = true;
}

void AAncientCharacter::Action_Sprint()
{
	bRunning = true;
	bRunningKeyPressed = true;

	if (GetCharacterMovement()->IsFlying())
	{
		GetCharacterMovement()->MaxFlySpeed = FlyRoughSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void AAncientCharacter::Action_StopSprint(bool bKeyStillPressed)
{
	bRunning = false;
	bRunningKeyPressed = bKeyStillPressed;

	if (GetCharacterMovement()->IsFlying())
	{
		GetCharacterMovement()->MaxFlySpeed = FlySpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AAncientCharacter::Action_Sneak()
{
	if (bRunning)
	{
		Action_StopSprint(true);
	}

	if (GetCharacterMovement()->IsFlying())
	{
		GetCharacterMovement()->MaxFlySpeed = SlowFlySpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = SlowWalkSpeed;
	}
}

void AAncientCharacter::Action_StopSneak()
{
	if (GetCharacterMovement()->IsFlying())
	{
		GetCharacterMovement()->MaxFlySpeed = FlySpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	if (bRunningKeyPressed)
	{
		Action_Sprint();
	}
}

void AAncientCharacter::Action_CrouchToggle()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void AAncientCharacter::Action_Ascend(float Value)
{
	if (ClimbComponent && ClimbComponent->IsHanging() == true)
	{
		return;
	}

	if (GetCharacterMovement()->IsFlying())
	{
		const FRotator Rotation = GetActorRotation();
		AddMovementInput(FRotationMatrix(Rotation).GetUnitAxis(EAxis::Z), Value);
	}
}

void AAncientCharacter::Action_Descend(float Value)
{
	if (ClimbComponent && ClimbComponent->IsHanging() == true)
	{
		return;
	}

	if (GetCharacterMovement()->IsFlying())
	{
		const FRotator Rotation = GetActorRotation();
		AddMovementInput(FRotationMatrix(Rotation).GetUnitAxis(EAxis::Z), Value * -1);
	}
}

void AAncientCharacter::Action_Interact()
{
	if (IsBusy() || GetCharacterMovement()->IsFlying())
	{
		return;
	}

	if (GetCharacterMovement()->IsWalking())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}

	bInteracting = true;
}

void AAncientCharacter::Action_InteractLoopStart()
{
	if (IsBusy() || GetCharacterMovement()->IsFlying())
	{
		return;
	}

	if (GetCharacterMovement()->IsWalking())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}

	bInteractingLoop = true;
	bInteractLoopAnim = true;
}

void AAncientCharacter::Action_InteractLoopEnd()
{
	bInteractingLoop = false;
}

void AAncientCharacter::Notify_InteractStop()
{
	bInteracting = false;
}

void AAncientCharacter::Notify_InteractLoopStop()
{
	bInteractLoopAnim = false;
}

void AAncientCharacter::Notify_InteractExecute()
{

}

void AAncientCharacter::Notify_InteractLoopExecute()
{

}