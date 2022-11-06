// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AncientCharacter.generated.h"

class UCameraComponent;
class UCapsuleComponent;
class URadarComponent;
class USphereComponent;
class USpringArmComponent;

class UClimbComponent;

/* Functions that start with the Notify_ called generally when the animation of the corresponding action is finished
 * See: EpicCharacterAnimInstance.h (you can see anim notifiers where these functions triggered)
 * 
 * Functions that start with the Action_ called generally when related key is pressed
 * See: Project Settings -> Input for view or edit input
*/

DECLARE_MULTICAST_DELEGATE(FOnPlayerMoved)
DECLARE_MULTICAST_DELEGATE(FOnPlayerTurned)

UCLASS(config=Game)
class AAncientCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Climbing")
	UClimbComponent* ClimbComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleCollision	= nullptr;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float TargetArmLength_Walk = 750;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float TargetArmLength_Run = 900;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float TargetArmLength_Fly = 1200;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float TargetArmLength_FlyRough = 1400;

public:

	FOnPlayerMoved OnPlayerMoved;
	FOnPlayerTurned OnPlayerTurned;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	float BaseTurnRate = 45.f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	float BaseLookUpRate = 45.f;
	
	UClimbComponent* GetClimbComponent() const { return ClimbComponent; }
	
	AAncientCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Crouch(bool bClientSimulation = false) override;
	
	virtual void UnCrouch(bool bClientSimulation = false) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Called for forwards/backward input */
	void Turn(float Value);

	/** Called for side to side input */
	void LookUp(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	
	/* ------------ Interactable ------------ */

	bool IsInteracting() const { return bInteracting; }
		
	bool IsInteractingLoop() const { return bInteractingLoop; }
		
	bool IsInteractingLoopAnim() const { return bInteractLoopAnim; }
		
	void Action_Interact();
			
	void Action_InteractLoopStart();
			
	void Action_InteractLoopEnd();
		
	void Notify_InteractExecute();	
			
	void Notify_InteractLoopExecute();
		
	void Notify_InteractStop();
		
	void Notify_InteractLoopStop();

	/* ------------ ~Interactable~ ------------ */

protected:

	/* ------------ Fly Mechanism ------------ */

	void Fly();

	void SetFly();

	void FloorCheck();

public:

	/* Check character have enough space when jumping or climbing */
	bool CeilCheck(FVector Loc);

protected:

	void Action_Ascend(float Value);

	void Action_Descend(float Value);

	/* ------------ ~Fly Mechanism~ ------------ */

public:

	void ResetJumpCounter() { JumpCounter = 0; }

	void Notify_CelebratingFinished(const bool InCelebrating) { bCelebrating = InCelebrating; }

	/* Called when jump animation finished (JumpEnd or JumpRunEnd anim notify triggered)*/
	void Notify_JumpFinished();

	bool IsCelebrating() const { return bCelebrating; }

	bool IsBusy() const;

protected:

	void Action_Jump();

	void Action_Celebrate();

	void Action_Sprint();
	
	void Action_StopSprint(bool bKey = false);

	void Action_Sneak();

	void Action_StopSneak();

	void Action_CrouchToggle();
		
private:

	int JumpCounter	= 0;

	bool bWantsToCrouch	= false;

	bool bRunningKeyPressed	= false;

	bool bRunning = false;

	bool bAscending = false;

	bool bDescending = false;

	bool bInteractLoopAnim = false;

	bool bCelebrating = false;

	bool bInteracting = false;

	bool bInteractingLoop = false;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float SlowWalkSpeed	= 300.f;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float CrouchWalkSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float WalkSpeed	= 800.f;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float RunSpeed = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float SlowFlySpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float FlySpeed = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float FlyRoughSpeed = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = Climbing)
	float SphereTraceRadius	= 60.0f;

	UPROPERTY()
	FTimerHandle Handle;

	/* ------------ Traces ------------ */
	UPROPERTY()
	TArray<AActor*>	IgnoredActors;

	FVector	Start = FVector::ZeroVector;
	FVector	End	= FVector::ZeroVector;

	FHitResult	OutHit;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	/* ------------ ~Traces~ ------------ */
};

