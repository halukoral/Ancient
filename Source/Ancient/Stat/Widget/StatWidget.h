// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatWidget.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;

struct FStat;

UCLASS()
class ANCIENT_API UStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void			UpdateStatusBar(const int32 InValue);
	
	void			InitializeBar(const FStat& InStat);
		
protected:

	virtual void	NativeConstruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar*	Bar					= nullptr ;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*		CurrentTextBlock	= nullptr ;

	UPROPERTY(meta = (BindWidget))
	UImage*			Icon				= nullptr ;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*		MaxTextBlock		= nullptr ;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor	BarColor;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D*		StatIcon			= nullptr ;

	UPROPERTY(EditDefaultsOnly)
	int32			MinValue			= 0;

	UPROPERTY(EditDefaultsOnly)
	int32			CurrentValue		= 0;

	UPROPERTY(EditDefaultsOnly)
	int32			MaxValue			= 0;

	UPROPERTY(EditDefaultsOnly)
	bool			bInit				= false;
};
