// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

enum class EProgressState : uint8;

UCLASS()
class ANCIENT_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void Init(const int32 InMax);

	void UpdateHealth(const int32 InHealth);

	void SetProgress(const EProgressState InState);

private:
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar	= nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*	HealthText = nullptr;

	EProgressState State;
	
	int32 Max = 0;
	int32 Current = 0;

};
