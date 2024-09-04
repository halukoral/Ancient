// Fill out your copyright notice in the Description page of Project Settings.

#include "RadarComponent.h"

#include "EngineUtils.h"
#include "Enums.h"
#include "Marker.h"
#include "Ancient/AncientCharacter.h"
#include "Ancient/Interactable/Interactable.h"
#include "Camera/CameraComponent.h"
#include "Components/Image.h"
#include "GameFramework/Character.h"
#include "Widgets/DirectionWidget.h"
#include "Widgets/EnemyWidget.h"
#include "Widgets/LandmarkWidget.h"
#include "Widgets/QuestWidget.h"
#include "Widgets/RadarWidget.h"

URadarComponent::URadarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	OnComponentBeginOverlap.AddDynamic(this, &URadarComponent::OnEnterPlayerSight);
	OnComponentEndOverlap.AddDynamic(this, &URadarComponent::OnLeavePlayerSight);
	
	Directions.Add(FDirection(TEXT("N"), 0));
	Directions.Add(FDirection(TEXT("E"), 90));
	Directions.Add(FDirection(TEXT("S"), 180));
	Directions.Add(FDirection(TEXT("W"), -90));
}

void URadarComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AAncientCharacter>(GetOwner());
	if (Player.IsValid())
	{
		Player->OnPlayerMoved.AddUObject(this, &URadarComponent::OnPlayerMoved);
		Player->OnPlayerTurned.AddUObject(this, &URadarComponent::OnPlayerTurned);
		
		PlayerCamera = Player->FindComponentByClass<UCameraComponent>();
		RadarWidget  = CreateWidget<URadarWidget>(GetWorld(), RadarWidgetClass);
		RadarWidget->AddToViewport();

		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors);
		for(const auto Actor : OverlappingActors)
		{
			if (ACharacter* Character = Cast<ACharacter>(Actor))
			{
				if (Character != Player)
				{
					AddEnemy(Character);			
				}
			}
		}
			
		AddDirections();
		AddQuests();
		AddLandmarks();
		OnBecomeVisible();	
	}
}

void URadarComponent::OnEnterPlayerSight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner())
	{
		if (ACharacter* Character = Cast<ACharacter>(OtherActor))
		{
			AddEnemy(Character);
		}
	}
}

void URadarComponent::OnLeavePlayerSight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != GetOwner())
	{
		if (ACharacter* Character = Cast<ACharacter>(OtherActor))
		{
			RemoveEnemy(Character);
		}
	}
}


float URadarComponent::RotToCircleDegree(const float Rotation) const
{
	return Rotation < 0.f ? FGenericPlatformMath::Fmod((Rotation + 360.f), 360.f) : Rotation;
}

float URadarComponent::GetDeltaClockwise(const float A, const float B, const bool bClockwise) const
{
	const float DegA = FGenericPlatformMath::Fmod(A, 360.f);
	const float DegB = FGenericPlatformMath::Fmod(B, 360.f);

	if (bClockwise)
	{
		if (DegA > DegB)
		{
			return 360.f - (DegA - DegB);
		}
		else if (DegA < DegB)
		{
			return (DegB - DegA);
		}
	}
	else
	{
		if (DegA > DegB)
		{
			return (DegA - DegB);
		}
		else if (DegA < DegB)
		{
			return 360.f - (DegB - DegA);
		}
	}
	return 0.f;
}

void URadarComponent::RotToTranslation(const FRotator& A, const FRotator& B, bool& bClockwise, bool& bInRadarSight, FVector2D& Translation) const
{
	const float CircleDegreeA = RotToCircleDegree(A.Yaw);
	const float CircleDegreeB = RotToCircleDegree(B.Yaw);

	const float DeltaClockwise = GetDeltaClockwise(CircleDegreeA, CircleDegreeB, true);
	const float Delta = GetDeltaClockwise(CircleDegreeA, CircleDegreeB, false);

	if (DeltaClockwise > Delta)
	{
		if (Delta / 90.f <= 1.0f)
		{
			bClockwise = false;
			bInRadarSight = true;
			Translation = FVector2D(
				-1.f * FMath::Lerp(0.f, MaxWidgetTranslation, Delta / 90.f),
				0.f
			);
		}
		else
		{
			bClockwise = false;
			bInRadarSight = false;
			Translation = FVector2D(0.f, 0.f);
		}
	}
	else if (DeltaClockwise <= Delta)
	{
		if (DeltaClockwise / 90.f <= 1.f)
		{
			bClockwise = true;
			bInRadarSight = true;
			Translation = FVector2D(
				FMath::Lerp(0.f, MaxWidgetTranslation, DeltaClockwise / 90.f),
				0.f
			);
		}
		else
		{
			bClockwise = true;
			bInRadarSight = false;
			Translation = FVector2D(0.f, 0.f);
		}
	}
}

int32 URadarComponent::DistanceToPlayer(const AActor* InActor) const
{
	const float DistanceX = InActor->GetActorLocation().X - Player->GetActorLocation().X;
	const float DistanceY = InActor->GetActorLocation().Y - Player->GetActorLocation().Y;
	const FVector Distance  = FVector(DistanceX, DistanceY, 0.f);
	return Distance.Size() / UnitsPerMeter;
}

void URadarComponent::OnBecomeVisible()
{
	UpdateDirectionWidgets();
	UpdateQuestDistance();
	UpdateQuestPosition();
	UpdateLandmarkPositions();
	UpdateDistancesForEnemies();
	UpdatePositionsForEnemies();
	UpdateQuestElevation();
}

void URadarComponent::SetRadarVisibility(const bool IsVisible, const bool CanAnimate)
{
	if (IsVisible != bVisible && bCanChangeVisibility)
	{
		bCanChangeVisibility = false;
		bVisible = IsVisible;
		if (CanAnimate)
		{
			FTimerHandle TimerHandle;
			if (bVisible)
			{
				OnBecomeVisible();
				RadarWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				VisibleAnimation();
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
					bCanChangeVisibility = true;
				}, DelayTime, false);
			}
			else
			{
				HideAnimation();
				// We can delay just like BP DELAY function
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
					RadarWidget->SetVisibility(ESlateVisibility::Collapsed);
					bCanChangeVisibility = true;
				}, DelayTime, false);
			}
		}
		else
		{
			if (bVisible)
			{
				OnBecomeVisible();
				RadarWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				RadarWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			bCanChangeVisibility = true;
		}
	}
}

void URadarComponent::ToggleRadarVisibility(const bool CanAnimate)
{
	SetRadarVisibility(!bVisible, CanAnimate);
}

void URadarComponent::OnPlayerMoved()
{
	if (bVisible)
	{
		UpdateQuestDistance();
		UpdateQuestPosition();
		UpdateLandmarkPositions();
		UpdateDistancesForEnemies();
		UpdatePositionsForEnemies();
		UpdateQuestElevation(); 
	}
}

void URadarComponent::OnPlayerTurned()
{
	if (bVisible)
	{
		UpdateDirectionWidgets();
		UpdateQuestPosition();
		UpdateLandmarkPositions();
		UpdatePositionsForEnemies();
	}
}

void URadarComponent::AddDirections()
{
	for(const FDirection& Direction : Directions)
	{
		RadarWidget->AddDirectionWidget(Direction);
	}
}

void URadarComponent::AddQuests()
{
	if (const UWorld* World = GetWorld())
	{
		for(TActorIterator<AMarker> It(World); It; ++It)
		{
			if (It->GetMarkerType() == EMarkerType::E_Quest && Landmarks.Contains(*It) == false)
			{
				AddQuest(*It);
			}			
		}
	}
}

void URadarComponent::AddQuest(AMarker* QuestMarker)
{
	Quests.Add(QuestMarker);
	if (RadarWidget.Get())
	{
		RadarWidget->AddQuestWidget(QuestMarker->GetQuestType());
	}
}

void URadarComponent::AddLandmarks()
{
	if (const UWorld* World = GetWorld())
	{
		for(TActorIterator<AMarker> It(World); It; ++It)
		{
			if (It->GetMarkerType() == EMarkerType::E_Landmark && Landmarks.Contains(*It) == false)
			{
				AddLandmark(*It);
			}			
		}
	}
}

void URadarComponent::AddLandmark(AMarker* Landmark)
{
	Landmarks.Add(Landmark);
	if (RadarWidget.Get())
	{
		RadarWidget->AddLandmarkWidget(Landmark->GetIcon());
	}
}

void URadarComponent::AddEnemy(ACharacter* Enemy)
{
	if (!Enemies.Contains(Enemy) && RadarWidget.IsValid())
	{
		Enemies.Add(Enemy);
		RadarWidget->AddEnemyWidget();
		UpdatePositionForEnemy(Enemy);
		UpdateDistanceForEnemy(Enemy);
	}
}

void URadarComponent::RemoveEnemy(ACharacter* Enemy)
{
	if (RadarWidget.IsValid())
	{
		const int32 EnemyIndex = Enemies.Find(Enemy);
		if (EnemyIndex >= 0)
		{
			Enemies.Remove(Enemy);
			RadarWidget->DeleteEnemyWidget(EnemyIndex);
		}
	}
}

void URadarComponent::OnEnemyMoved(ACharacter* Enemy) const
{
	UpdateDistanceForEnemy(Enemy);
	UpdatePositionForEnemy(Enemy);
}

void URadarComponent::UpdateQuestDistance()
{
	if (RadarWidget.IsValid())
	{
		int i = 0;
		for (const UQuestWidget* QuestWidget : RadarWidget->GetQuestWidgets())
		{
			FVector QuestLocation = Quests[i]->GetActorLocation();
			FVector PlayerLocation = Player->GetActorLocation();
			FVector DifferenceLocation = QuestLocation - PlayerLocation;
			DifferenceLocation.Z = 0.f;
			const float DifferenceLength = DifferenceLocation.Size();
			QuestWidget->UpdateDistance(DifferenceLength / UnitsPerMeter);
			++i;
		}
	}
}

void URadarComponent::UpdateQuestPosition()
{
	if (Player.IsValid() && RadarWidget.IsValid())
	{
		int i = 0;
		for (UQuestWidget* QuestWidget : RadarWidget->GetQuestWidgets())
		{
			FRotator CameraRotation = PlayerCamera->GetComponentRotation();
			FVector  Start = Player->GetActorLocation();
			FVector  Target = Quests[i]->GetActorLocation();
			FRotator LookAtRotation = FRotationMatrix::MakeFromX(Target - Start).Rotator();
			
			bool IsClockwise;
			bool IsInRadarSight;
			FVector2D TranslationResult;
			RotToTranslation(CameraRotation, LookAtRotation, IsClockwise, IsInRadarSight, TranslationResult);
			
			if (bHideQuestMarkerWhenOutOfSight)
			{
				if (IsInRadarSight)
				{
					QuestWidget->SetRenderTranslation(TranslationResult);
					if (!QuestWidget->IsVisible())
					{
						QuestWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					}
				}
				else
				{
					if (QuestWidget->IsVisible())
					{
						QuestWidget->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
			}
			else
			{
				if (IsInRadarSight)
				{
					QuestWidget->SetRenderTranslation(TranslationResult);
				}
				else
				{
					if (IsClockwise)
					{
						QuestWidget->SetRenderTranslation(FVector2D(MaxWidgetTranslation, 0.f));
					}
					else
					{
						QuestWidget->SetRenderTranslation(FVector2D(MaxWidgetTranslation * -1, 0.f));
					}
				}
			}
			++i;
		}
	}
}

void URadarComponent::UpdateQuestElevation()
{
	if (Player.IsValid() && RadarWidget.IsValid())
	{
		int i = 0;
		for (const UQuestWidget* QuestWidget : RadarWidget->GetQuestWidgets())
		{
			const float QuestElevation  = Quests[i]->GetActorLocation().Z;
			const float PlayerElevation = Player->GetActorLocation().Z;

			if (QuestElevation > PlayerElevation + DefaultElevationRange)
			{
				QuestWidget->SetArrowDirection(true);
				QuestWidget->GetArrow()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else if (QuestElevation < PlayerElevation - DefaultElevationRange)
			{
				QuestWidget->SetArrowDirection(false);
				QuestWidget->GetArrow()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				QuestWidget->GetArrow()->SetVisibility(ESlateVisibility::Collapsed);
			}
			++i;
		}
	}
}

void URadarComponent::OnEnterSecretRange(AInteractable* Interactable)
{
	if (RadarWidget.IsValid() && Interactable)
	{
		if (!Interactables.Contains(Interactable))
		{
			Interactables.Add(Interactable);
			if (Interactables.Num() >= 1)
			{
				RadarWidget->HighlightRadar(true);
			}
		}		
	}
}

void URadarComponent::OnLeaveSecretRange(AInteractable* Interactable)
{
	if (RadarWidget.IsValid() && Interactable)
	{
		if (Interactables.Contains(Interactable))
		{
			Interactables.Remove(Interactable);
			if (Interactables.Num() == 0)
			{
				RadarWidget->HighlightRadar(false);
			}
		}	
	}
}

void URadarComponent::UpdatePositionForEnemy(ACharacter* Enemy) const
{
	const int32 EnemyIndex = Enemies.Find(Enemy);
	if (EnemyIndex >= 0)
	{
		const FRotator CameraRotation = PlayerCamera->GetComponentRotation();
		const FVector Start = Player->GetActorLocation();
		const FVector Target = Enemy->GetActorLocation();
		const FRotator LookAtRotation = FRotationMatrix::MakeFromX(Target - Start).Rotator();

		bool IsClockwise;
		bool IsInRadarSight;
		FVector2D TranslationResult;
		RotToTranslation(CameraRotation, LookAtRotation, IsClockwise, IsInRadarSight, TranslationResult);
		
		if (RadarWidget.IsValid())
		{
			const TArray<UEnemyWidget*> EnemyWidgets = RadarWidget->GetEnemyWidgets();
			if (EnemyWidgets.Num() > 0)
			{
				UEnemyWidget* EnemyWidget = EnemyWidgets[EnemyIndex];
				if (IsInRadarSight)
				{
					EnemyWidget->SetRenderTranslation(TranslationResult);
					if (!EnemyWidget->IsVisible())
					{
						EnemyWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					}
				}
				else
				{
					if (EnemyWidget->IsVisible())
					{
						EnemyWidget->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
			}
		}
	}
}

void URadarComponent::UpdateDistanceForEnemy(ACharacter* Enemy) const
{
	const int32 EnemyIndex = Enemies.Find(Enemy);
	if (EnemyIndex >= 0)
	{
		if (RadarWidget.IsValid())
		{
			const TArray<UEnemyWidget*> EnemyWidgets = RadarWidget->GetEnemyWidgets();
			if (EnemyWidgets.Num() > 0)
			{
				if (const UEnemyWidget* EnemyWidget = EnemyWidgets[EnemyIndex]) 
				{
					EnemyWidget->UpdateDistance(DistanceToPlayer(Enemy));
				}
			}
		}
	}
}

void URadarComponent::UpdatePositionsForEnemies()
{
	for (const auto Enemy : Enemies)
	{
		UpdatePositionForEnemy(Enemy);
	}
}

void URadarComponent::UpdateDistancesForEnemies()
{
	for (const auto Enemy : Enemies)
	{
		UpdateDistanceForEnemy(Enemy);
	}
}

void URadarComponent::UpdateLandmarkPositions()
{
	if (RadarWidget.IsValid())
	{
		int i = 0;
		for (ULandmarkWidget* LandmarkWidget : RadarWidget->GetLandmarkWidgets())
		{
			FRotator CameraRotation = PlayerCamera->GetComponentRotation();
			FVector Start = Player->GetActorLocation();
			FVector Target = Landmarks[i]->GetActorLocation();
			FRotator LookAtRotation = FRotationMatrix::MakeFromX(Target - Start).Rotator();
			
			bool IsClockwise;
			bool IsInRadarSight;
			FVector2D TranslationResult;
			RotToTranslation(CameraRotation, LookAtRotation, IsClockwise, IsInRadarSight, TranslationResult);

			if (IsInRadarSight)
			{
				LandmarkWidget->SetRenderTranslation(TranslationResult);
				if (!LandmarkWidget->IsVisible())
				{
					LandmarkWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
			else
			{
				if (LandmarkWidget->IsVisible())
				{
					LandmarkWidget->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			++i;
		}
	}
}

void URadarComponent::UpdateDirectionWidgets()
{
	if (RadarWidget.IsValid())
	{
		int i = 0;
		for (UDirectionWidget* DirectionWidget : RadarWidget->GetDirectionWidgets())
		{
			FRotator CameraRotation  = PlayerCamera->GetComponentRotation();
			const int32 DirectionYaw = Directions[i].WorldRotation;

			bool IsClockwise;
			bool IsInRadarSight;
			FVector2D TranslationResult;
			RotToTranslation(CameraRotation, FRotator(0.f, DirectionYaw, 0.f), IsClockwise, IsInRadarSight, TranslationResult);
			
			if (IsInRadarSight)
			{
				DirectionWidget->SetRenderTranslation(TranslationResult);
				if (!DirectionWidget->IsVisible())
				{
					DirectionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
			else
			{
				if (DirectionWidget->IsVisible())
				{
					DirectionWidget->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			++i;
		}
	}
}
