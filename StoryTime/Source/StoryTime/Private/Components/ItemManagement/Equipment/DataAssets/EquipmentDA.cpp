// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ItemManagement/Equipment/DataAssets/EquipmentDA.h"

void UEquipmentDA::Initialize()
{
	Super::Initialize();
	// Get the equipment curves
	Stats.Empty();
	const auto links = FindLinks("parameterCurves", "equipmentId", ID);
	for (const auto& Link : links)
	{
		const auto row = Link.Value;
		if (row->Fields.Num() == 0)
			continue;

		FEquipmentStat stat = FEquipmentStat();

		for(auto& Field : row->Fields)
		{
			if (Field.Value.Data == nullptr)
				continue;

			if (Field.Key == "base" || Field.Key == "flat")
			{
				float d;
				memcpy(&d, Field.Value.Data.Get(), Field.Value.Size);

				stat.Flat = d;
			}

			if (Field.Key == "alias")
			{
				std::string dest;
				memcpy((void*)dest.c_str(), Field.Value.Data.Get(), Field.Value.Size);
				stat.Alias = UTF8_TO_TCHAR(dest.c_str());
			}

			if (Field.Key == "statType")
			{
				uint8 d;
				memcpy(&d, Field.Value.Data.Get(), Field.Value.Size);
				stat.StatType = static_cast<StatModType>(d);
			}
		}

		Stats.Add(stat);
	}
}

void UEquipmentDA::OnTableIDChanged()
{
	Super::OnTableIDChanged();
	Initialize();
}
