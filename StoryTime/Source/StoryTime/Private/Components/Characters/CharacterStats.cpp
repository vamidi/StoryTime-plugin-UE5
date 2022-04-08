#include "Components/Characters/CharacterStats.h"

void FCharacterStats::Add(const FStatModifier& Modifier)
{
	m_bIsDirty = true;
	StatModifiers.Add(Modifier);
	StatModifiers.Sort([] (const FStatModifier& a, const FStatModifier& b)
	{
		return a.GetOrder() > b.GetOrder();
		/*
		if (a.GetOrder() < b.GetOrder())
			return -1;
		*/

		/*
		if (a.GetOrder() > b.GetOrder())
			return 1;

		return 0;
		*/
	});
}

bool FCharacterStats::Remove(const FStatModifier& Modifier)
{
	StatModifiers.Remove(Modifier);
	return m_bIsDirty;
}

void FCharacterStats::Clear()
{
	StatModifiers.Empty();
}

float FCharacterStats::Calculate()
{
	float finalValue = BaseValue;
	float sumPercentAdd = 0;

	for (int i = 0; i < StatModifiers.Num(); i++)
	{
		auto& mod = StatModifiers[i];
		switch (mod.GetType())
		{
		case StatModType::Flat:
			finalValue += mod.GetValue();
			break;
		case StatModType::PercentAdd:
			sumPercentAdd += mod.GetValue();
			if (i + 1 >= StatModifiers.Num() || StatModifiers[i + 1].GetType() != StatModType::PercentAdd)
			{
				finalValue *= 1 + sumPercentAdd;
				sumPercentAdd = 0;
			}
			break;
		case StatModType::PercentMult:
			finalValue *= 1 + mod.GetValue();
			break;
		case StatModType::None:
		default:
			break;
		}
	}

	// 12.0001f != 12f
	return FMath::RoundToFloat(finalValue);
}
