#pragma once
#include "SlateBasics.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraphSchema_K2.h"
#include "EdGraphUtilities.h"
#include "DatabaseIDPin.h"
#include "DatabaseIDPinGraph.h"

class STORYTIMEEDITOR_API DatabaseIDPinGraphFactory : public FGraphPanelPinFactory
{
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* InPin) const override
	{
		const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

		if (InPin->PinType.PinCategory == K2Schema->PC_Struct && InPin->PinType.PinSubCategoryObject == FDatabaseIDPin::StaticStruct())
		{
			return SNew(SDatabaseIDPinGraph, InPin);
		}
		return nullptr;
	}
};
