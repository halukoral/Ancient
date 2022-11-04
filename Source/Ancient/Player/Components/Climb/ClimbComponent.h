// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ClimbComponent.generated.h"

class UArrowComponent;
class AAncientCharacter;

UCLASS( Blueprintable )
class ANCIENT_API UClimbComponent : public USceneComponent
{
	GENERATED_BODY()

public:	

	UClimbComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupInput(class UInputComponent* PlayerInputComponent);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

protected:

	virtual void BeginPlay() override;

	void HeightTracer();

	void ForwardTracer(AActor* ClimbableActor);

	void LeftTracer();

	void RightTracer();

	bool LeftJumpTracer();

	bool RightJumpTracer();

	void LeftCornerTracer();

	void RightCornerTracer();

	void JumpUpTracer();

	void HangingLeft();

	void HangingRight();

	void HangingMove(FVector InStart, FVector InEnd);

	void JumpBack();

	void GrabLedge();

	UFUNCTION(BlueprintCallable)
	void Grabbed();

	/* Called when player press the jump button again
	 * while hanging and will trigger to climbing animations */
	void ClimbLedge();

	/* Called when player press the cancel button
	 * while hanging and will trigger to falling animations	*/
	void Action_DropLedge();

public:

	void Jump();

	AActor* GetClingingActor() const { return ClingingActor.Get(); }

	void ResetClimbActors();
	void ResetMoveRight();
	void ResetMoveForward();

	bool IsClimbing() const { return bClimbing; }
	bool IsHanging() const { return bHanging; }
	bool IsJumpUp() const { return bJumpUp; }
	bool IsJumpBack() const { return bCanJumpBack; }
	bool IsHangingLeft() const { return bHangingLeft; }
	bool IsHangingRight() const { return bHangingRight; }
	bool IsHangingJumpLeft() const { return bHangingJumpLeft; }
	bool IsHangingJumpRight() const { return bHangingJumpRight; }
	bool IsHangingCornerLeft() const { return bHangingCornerLeft; }
	bool IsHangingCornerRight() const { return bHangingCornerRight; }

	void Notify_JumpUp(bool InValue);
	void Notify_HangingJumpLeft(bool InValue);
	void Notify_HangingJumpRight(bool InValue);
	void Notify_HangingCornerLeft(bool InValue);
	void Notify_HangingCornerRight(bool InValue);
	void Notify_HangingFinished();

private:
	
	UPROPERTY(VisibleAnywhere, Category = Climbing)
	UArrowComponent* LeftArrow = nullptr;

	/* Used when character move left while hanging for detecting rotation of ledge
	 * Should be very close to the capsule, otherwise rotation deviation increases
	 */
	UPROPERTY(VisibleAnywhere, Category = Climbing)
	UArrowComponent* LeftShoulderArrow = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Climbing)
	UArrowComponent* LeftEdgeArrow = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Climbing)
	UArrowComponent* RightArrow = nullptr;

	/* Used when character move left while hanging for detecting rotation of ledge
	 * Should be very close to the capsule, otherwise rotation deviation increases
	 */
	UPROPERTY(VisibleAnywhere, Category = Climbing)
	UArrowComponent* RightShoulderArrow = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Climbing)
	UArrowComponent* RightEdgeArrow = nullptr;

	// Coefficient for how far away from the wall our capsule
	// you should tweak with this value based on model and trace method
	// higher values -> farther || lower values -> closer
	UPROPERTY(EditDefaultsOnly, Category = Climbing)
	float DistToWallCoeff = 10.f;

	// Coefficient for how fast move side while hanging
	// higher values -> faster || lower values -> slower
	UPROPERTY(EditDefaultsOnly, Category = Climbing)
	float HangingSideCoeff = 40.f;

	UPROPERTY(EditDefaultsOnly, Category = Climbing)
	float SphereTraceRadius = 60.0f;

	// do not forget to create socket on pelvis bone 
	UPROPERTY(EditDefaultsOnly, Category = Climbing)
	FName SocketName = "pelvisSocket";

	FVector WallNormal = FVector::ZeroVector;

	FVector WallLocation = FVector::ZeroVector;

	FVector HeightLocation = FVector::ZeroVector;

	// we need this to not hold onto the same ledge while jumping from the ledge
	TWeakObjectPtr<AActor> ClingingActor = nullptr;

	TWeakObjectPtr<AAncientCharacter> Character = nullptr;

	// For FLatentActionInfo
	int32 NextUUID = 1;

	int32 GetNextUUID() { return NextUUID++; }

	/* ------------ Traces ------------ */
	UPROPERTY()
	TArray<AActor*> IgnoredActors;

	FVector Start = FVector::ZeroVector;
	FVector End	= FVector::ZeroVector;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	/* ------------ ~Traces~ ------------ */

	bool bHanging = false;

	bool bTryJumpOrTurn = false;

	bool bJumpUp = false;

	bool bCanJumpUp	= false;

	bool bCanJumpBack = false;

	bool bClimbing = false;

	/* Left */
	bool bHangingLeft = false;

	bool bCanHangingJumpLeft = false;

	bool bHangingJumpLeft = false;

	bool bCanHangingCornerLeft = false;

	bool bHangingCornerLeft = false;

	/* Right */
	bool bHangingRight = false;

	bool bCanHangingJumpRight = false;

	bool bHangingJumpRight = false;

	bool bCanHangingCornerRight = false;

	bool bHangingCornerRight = false;
};
