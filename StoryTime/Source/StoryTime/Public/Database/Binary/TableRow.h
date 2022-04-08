#pragma once

#include "CoreMinimal.h"

#include "TableRowInfo.h"
#include "TableField.h"

#include "TableRow.generated.h"

USTRUCT(BlueprintType)
struct FTableRow
{
	GENERATED_BODY()
public:
	uint32_t RowId = UINT_MAX;

	TMap<FTableRowInfo, FTableField> Fields;
};
