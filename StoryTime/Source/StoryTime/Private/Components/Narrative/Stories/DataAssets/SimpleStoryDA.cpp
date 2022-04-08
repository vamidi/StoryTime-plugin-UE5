#include "Components/Narrative/Stories/DataAssets/SimpleStoryDA.h"

void USimpleStoryDA::Reset()
{
	Super::Reset();

	ChildId = UINT_MAX;
}

void USimpleStoryDA::ConvertRow()
{
	Super::ConvertRow();

	const auto row = GetRow(GetTableName(), ID);
	if (row.Fields.Num() == 0) {
		return;
	}

	for (auto& Field : row.Fields)
	{
		// Fetch the first dialogue we should start
		if (Field.Key == "childId")
		{
			// retrieve the necessary items
			int32 d;
			memcpy(&d, Field.Value.Data.Get(), Field.Value.Size);

			ChildId = d;
		}

		if (Field.Key == "parentId")
		{
			// retrieve the necessary items
			int32 d;
			memcpy(&d, Field.Value.Data.Get(), Field.Value.Size);

			ParentId = d;
		}

		if (Field.Key == "typeId")
		{
			int32 d;
			memcpy(&d, Field.Value.Data.Get(), Field.Value.Size);

			// TypeId = (StoryType) field.Value.Data;
		}
	}
}
