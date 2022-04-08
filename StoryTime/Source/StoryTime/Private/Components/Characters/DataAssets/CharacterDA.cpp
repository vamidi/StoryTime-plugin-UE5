// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/DataAssets/CharacterDA.h"

#include "Components/Characters/CharacterStats.h"
#include "Components/Characters/StatModifier.h"
#include "Components/Characters/DataAssets/CharacterClassDA.h"
#include "Components/ItemManagement/Equipment/DataAssets/EquipmentDA.h"

void UCharacterDA::Equip(const UEquipmentDA* equipment) const
{
	// loop through all the stats that the equipment has.
	for (auto stat : equipment->GetStats())
	{
		// find the alias in the character to see if we have an equipment that updates or downgrades the stat.
		if(FCharacterStats* CharacterStat = CharacterClass->Find(stat.Alias)) {
			CharacterStat->Add(FStatModifier(stat.Flat, stat.StatType/*, this */));
		}
	}
}

TArray<TSoftObjectPtr<UEquipmentDA>> UCharacterDA::GetEquipments()
{
	return equipments;
}
