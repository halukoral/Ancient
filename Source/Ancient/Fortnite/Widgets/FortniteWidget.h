// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FortniteWidget.generated.h"

class UImage;
class USizeBox;
class UTextBlock;
class UHorizontalBox;

class UPlaceableManager;
class UPlaceableWidget;
class UResourceManager;
class UResourceWidget;

enum class EPlayerMode : uint8;
enum class EResourceTypes : uint8;

UCLASS()
class ANCIENT_API UFortniteWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void			SetResourceManager(UResourceManager* InResourceManager) { ResourceManager = InResourceManager; }

	void			SetPlaceableManager(UPlaceableManager* InPlaceableManager) { PlaceableManager = InPlaceableManager; }

	void			ShowCurrentResource(bool bShowResource) const;

	void			SetWidgetMode(const EPlayerMode PlayerMode) const;

	void			GenerateResourceWidgets();

	void			GeneratePlaceableWidgets();

	void			UpdateCurrentResource() const;

	UFUNCTION()
	void			UpdateResourceAmount(const EResourceTypes InType, const int32 InAmount);
	
private:
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox*		BuildingBox			= nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage*				CrossHair			= nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*			CurrentResourceAmount = nullptr;

	UPROPERTY(meta = (BindWidget))
	USizeBox*			CurrentResourceBox	= nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage*				CurrentResourceIcon	= nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage*				StateIcon			= nullptr;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox*		ResourceBox			= nullptr;

	UResourceManager*	ResourceManager		= nullptr;

	UPlaceableManager*	PlaceableManager	= nullptr;
	
	UResourceWidget*	W_Resource			= nullptr;
	
	UPlaceableWidget*	W_Placeable			= nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D*			CrossHair_Combat	= nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D*			CrossHair_Construct	= nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D*			IconCombat			= nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D*			IconConstruct		= nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UResourceWidget> ResourceWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlaceableWidget> PlaceableWidgetClass = nullptr;

	TMap<EResourceTypes, UResourceWidget*> ResourceWidgets;

};
