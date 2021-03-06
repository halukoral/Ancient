// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceManager.generated.h"

class UDataTable;
class UResourceWidget;
enum class EResourceTypes : uint8;
struct FResourceData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResourceChanged, const EResourceTypes, ResourceType, const int32, ResourceAmount);

UCLASS( Blueprintable )
class ANCIENT_API UResourceManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UResourceManager();

	FResourceChanged OnResourceChanged;

protected:
	
	virtual void	BeginPlay() override;

public:
	
	UDataTable* 	GetResourceDataTable() const   { return ResourceDataTable; }

	EResourceTypes	GetCurrentResourceType() const { return CurrentResource; }

	FResourceData*	GetResourceData() const;
	
	int32			GetResourceAmount(const EResourceTypes Type) const { return Resources[Type]; }

	int32			GetCurrentResourceAmount() const { return Resources[CurrentResource]; }
	
	void			AddResource(const int32 Amount);

	void			RemoveResource(const int32 Amount);
	
	void			UpdateResourceWidget(const EResourceTypes Type) const;

	void			AddWidgets(const EResourceTypes Type, UResourceWidget* Widget) { ResourceWidgets.Add(Type, Widget); }
	
	void			SelectNextWidget();

private:

	void			ToggleWidget(const bool InValue);
	
	void			ChangeResourceType();

	int32 MinValue = 0;
	int32 MaxValue = 999;

	EResourceTypes CurrentResource;

	TMap<EResourceTypes, int32> Resources;
	
	TMap<EResourceTypes, UResourceWidget*> ResourceWidgets;
	
	UPROPERTY(EditDefaultsOnly, Category = "Resource Manager | Data Table")
	UDataTable* ResourceDataTable = nullptr;
};
