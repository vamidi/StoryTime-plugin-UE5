#pragma once

#include "ItemTypeDA.generated.h"

UCLASS()
class STORYTIME_API UItemTypeDA : public UPrimaryDataAsset
{
public:
	GENERATED_BODY()

protected:
	/** The action associated with the item type. */
	UPROPERTY(BlueprintReadOnly)
	FText /* LocalizedString */ ActionName;

	/** The action associated with the item type. */
	UPROPERTY(BlueprintReadOnly)
	FText /* LocalizedString */ TypeName;

	/** The Item's background color in the UI */
	UPROPERTY(BlueprintReadOnly)
	FColor TypeColor;

	/** The Item's type. */
	// UPROPERTY(BlueprintReadOnly)
	// ItemInventoryType Type;

	/** The Item's action type. */
	// UPROPERTY(BlueprintReadOnly)
	// ItemInventoryActionType ActionType;

	/** The tab type under which the item will be added. */
	// UPROPERTY(BlueprintReadOnly)
	// InventoryTabTypeSO TabType;
};
