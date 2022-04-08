#pragma once

#include "ItemStack.generated.h"

class UTableDA;

USTRUCT(BlueprintType)
struct STORYTIME_API FItemStack
{
	GENERATED_USTRUCT_BODY()

	FItemStack() {}
	// FItemStack(const FItemStack& item);
	// FItemStack(const FItemStack&& item) noexcept;
	FItemStack(const UTableDA* item, int32 amount, int32 maxAmount = 99);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTableDA> Item = nullptr;

	/** Amount we are going to give to the player. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Amount = 0;

	/** Enable max amount of items we can have on this item. */
	UPROPERTY(EditDefaultsOnly)
	bool SetMax = false;

	/** Max amount we can carry of this item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAmount = 0;
};
