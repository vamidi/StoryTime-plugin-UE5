
#include "Components/Characters/DataAssets/CharacterClassDA.h"

#include "Components/Characters/CharacterStats.h"


FCharacterStats* UCharacterClassDA::Find(const FName& alias)
{
	return CharacterStats.FindByPredicate([alias] (const FCharacterStats& stat)
	{
		return stat.GetAlias() == alias;
	});
}

void UCharacterClassDA::OnTableIDChanged()
{
	Super::OnTableIDChanged();
}

void UCharacterClassDA::Initialize()
{
	if (ID != UINT_MAX)
	{
		// Clear out every stat modifier the player class has.
		ClearModifiers();

		// collection = overrideTable ? collection : LocalizationEditorSettings.GetStringTableCollection("Class Names");
		// Only get the first dialogue.
		const auto entryId = FString::FromInt(ID + 1);
		// if(collection)
			// className = new LocalizedString { TableReference = collection.TableCollectionNameReference, TableEntryReference = entryId };
		// else
			// Debug.LogWarning("Collection not found. Did you create any localization tables for Classes");

		CharacterStats.Empty();
		const auto links = FindLinks("parameterCurves", "classId", ID);
		for (auto& link : links)
		{
			const FTableRow* row = link.Value;
			if (row->Fields.Num() == 0)
				continue;

			FCharacterStats stat = FCharacterStats();
			stat.ID = link.Key;
#if UE_EDITOR
			// collection = overrideTable ? collection : LocalizationEditorSettings.GetStringTableCollection("Stat Names");
#endif
			// stat = CharacterStats.StatTable.ConvertRow(row, collection, stat);

			CharacterStats.Add(stat);
		}
	}
}

void UCharacterClassDA::ClearModifiers()
{
	for (auto& stat : CharacterStats)
	{
		if (stat.GetStatModifiers().Num() == 0)
			continue;

		stat.Clear();
	}
}
