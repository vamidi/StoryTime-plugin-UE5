// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ItemManagement/Inventory/DataAssets/ItemDA.h"

void UItemDA::Initialize()
{
	auto entryId = FString::FromInt(ID + 1);
	// collection = LocalizationEditorSettings.GetStringTableCollection("Item Names");
	// if (collection != null)
		// itemName = new LocalizedString { TableReference = collection.TableCollectionNameReference, TableEntryReference = entryId };
	// else
		// Debug.LogWarning("Collection not found. Did you create any localization tables for Items");

	// var descriptionCollection = overrideDescriptionTable ? itemDescriptionCollection : LocalizationEditorSettings.GetStringTableCollection("Item Descriptions");
	// if (descriptionCollection != null)
		// description = new LocalizedString { TableReference = descriptionCollection.TableCollectionNameReference, TableEntryReference = entryId };
	// else
		// GLog->Log(ELogVerbosity::Warning, TEXT("Collection not found. Did you create any localization tables for Items"));
}
