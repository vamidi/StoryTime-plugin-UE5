#pragma once

#include "StatModifier.generated.h"

/**
 * @source https://youtu.be/SH25f3cXBVc
 */
UENUM()
enum class StatModType
{
	None = 0			UMETA(DisplayName = "None"),
	Flat = 100			UMETA(DisplayName = "Flat"), // Flexible to create values in between values if the user wants to.
	PercentAdd = 200	UMETA(DisplayName = "PercentAdd"),
	PercentMult = 300	UMETA(DisplayName = "PercentMult")
};

USTRUCT(BlueprintType)
struct STORYTIME_API FStatModifier
{
	GENERATED_USTRUCT_BODY()

	FStatModifier() { }

	FStatModifier(float value, StatModType type, int index /*, object source = nullptr */)
	{
		Value = value;
		StatType = type;
		Order = index;
		// Source = source;
	}

	FStatModifier(float value, StatModType type): FStatModifier(value, type, static_cast<int32>(type)) { }
	// FStatModifier(float value, StatModType type, object source): FStatModifier(value, type, static_cast<int32>(type), source) { }

	//override the "<" operator so that this object can be sorted
	FORCEINLINE bool operator<(const FStatModifier &Other) const
	{
		return Order < Other.Order;
	}

	//check to see if the aggro record matches another aggro record by overloading the "==" operator.
	FORCEINLINE bool operator==(const FStatModifier &Other) const
	{
		return StatType == Other.StatType && Order == Other.Order;
	}

	float GetValue() const { return Value; }
	StatModType GetType() const { return StatType; }
	int32 GetOrder() const { return Order; }

	// Useful for sharing information what is providing the modifier.
	// public readonly object Source;

private:
	float Value = 0.f;
	StatModType StatType = StatModType::None;
	int32 Order = 0;
};
