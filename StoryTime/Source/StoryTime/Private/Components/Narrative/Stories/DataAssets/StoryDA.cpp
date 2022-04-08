// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Narrative/Stories/DataAssets/StoryDA.h"

#include "Database/TableDatabase.h"

void UStoryDA::ConvertRow()
{
	Super::ConvertRow();

	const auto row = GetRow(GetTableName(), ID);
	if (row.Fields.Num() == 0) {
		return;
	}

	for (auto& Field : row.Fields)
	{
		if (Field.Key == "typeId")
		{
			int32 d;
			memcpy(&d, Field.Value.Data.Get(), Field.Value.Size);

			TypeId = static_cast<EStoryType>(d);
		}
	}
}

void UStoryDA::Initialize()
{
	if(ID != UINT_MAX && ChildId != UINT_MAX) {
		// DialogueLines.Clear();

		const auto entryId = FString::FromInt(ID + 1);

		const auto* Field = TableDatabase::Get().GetField(GetTableName(), "data", ID);
		if (Field)
		{
			// Parse story data
			// StoryTable.ParseNodeData(this, (JObject) Field.Data);
		}
	}
}
