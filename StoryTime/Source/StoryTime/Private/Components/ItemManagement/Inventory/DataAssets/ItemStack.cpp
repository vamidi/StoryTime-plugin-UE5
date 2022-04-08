#include "Components/ItemManagement/Inventory/DataAssets/ItemStack.h"

#include "Components/Base/DataAssets/TableDA.h"

/*
FItemStack::FItemStack(const FItemStack& item)
{
	Item = item.Item;
	Amount = item.Amount;
	MaxAmount = item.MaxAmount;
}

FItemStack::FItemStack(const FItemStack&& item) noexcept
{
	Item = std::move(item.Item);
	Amount = std::move(item.Amount);
	MaxAmount = std::move(item.MaxAmount);
}
*/

FItemStack::FItemStack(const UTableDA* item, int32 amount, int32 maxAmount)
{
	// TODO maybe initialize new pointer
	Item = item;
	Amount = amount;
	MaxAmount = maxAmount;
}
