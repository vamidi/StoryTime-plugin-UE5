#pragma once

#include "TableData.Generated.h"

struct FTableRow;

USTRUCT(BlueprintType)
struct FTableData
{
	GENERATED_BODY()
	TMap<uint32, FTableRow> Rows = TMap<uint32, FTableRow>();
};

