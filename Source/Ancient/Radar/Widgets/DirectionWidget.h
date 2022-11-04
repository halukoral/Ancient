// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Ancient/Radar/Structs/Direction.h"
#include "Blueprint/UserWidget.h"
#include "DirectionWidget.generated.h"

class UTextBlock;

UCLASS()
class ANCIENT_API UDirectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeConstruct() override;

public:
	
	void SetDirection(const FDirection& NewDirectionInfo) { Direction = NewDirectionInfo; }
	const FDirection& GetDirection() const { return Direction; }

private:
	
	FDirection Direction;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text = nullptr;
};
