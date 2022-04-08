// Fill out your copyright notice in the Description page of Project Settings.
#include "Database/TableDatabase.h"

#include "JsonObjectConverter.h"
#include "Interfaces/IPluginManager.h"

#include "Components/Base/DataAssets/TableDA.h"
#include "Database/Binary/TableBinary.h"
#include "Database/Binary/TableData.h"
#include "HAL/FileManagerGeneric.h"

TableDatabase* TableDatabase::s_Instance = nullptr;
TOptional<FDatabaseConfig> TableDatabase::Config = TOptional<FDatabaseConfig>();
TableDatabase& TableDatabase::Get()
{
	if(s_Instance == nullptr) {
		s_Instance = new TableDatabase();
	}

	return *s_Instance;
}

const FDatabaseConfig& TableDatabase::Fetch()
{
	// Load it.
	if ( !Config.IsSet())
	{
		FString Json;
		const auto configFilePath = FPaths::ConvertRelativePathToFull(IPluginManager::Get().FindPlugin(TEXT("StoryTimeEditor"))->GetBaseDir()) / TEXT("Resources/config/config.json");
		if ( FFileHelper::LoadFileToString(Json, *configFilePath) )
		{
			FDatabaseConfig configFile;
			if ( FJsonObjectConverter::JsonObjectStringToUStruct(Json, &configFile, 0, 0) )
			{
				Config = configFile;
			}
			else {
				GLog->Log(ELogVerbosity::Error, "Couldn't deserialize config file!");
			}
		}
	}

	return Config.GetValue();
}

TableDatabase::TableDatabase()
{
#if WITH_EDITOR
	Refresh();
#endif
}

TableDatabase::~TableDatabase()
{
}

const FTableData& TableDatabase::GetTable(const FString& tableName)
{
	const TableBinary* table = (Data.Find(tableName));

	if (!table) {
		table = &Data.Add(tableName, {});

		ensure(table);
	}
	return table->GetTable();
}

TableBinary& TableDatabase::GetBinary(const FString& tableName)
{
	TableBinary* binary = Data.Find(tableName);

	if (!binary)
	{
		TableBinary bin = TableBinary({ TCHAR_TO_UTF8(*tableName) });
		binary = &Data.Add(tableName, bin);
	}

	return *binary;
}

const FTableRow& TableDatabase::GetRow(const FString& tableName, uint32 ID)
{
	FTableData& table = const_cast<FTableData&>(GetTable(tableName));
	FTableRow* row = table.Rows.Find(ID);

	if (!row)
	{
		FString errorMsg = tableName;
		errorMsg.Append(": Couldn't find row ").Append(ID + "in table!");
		GLog->Log(ELogVerbosity::Warning, errorMsg);

		const FTableRow r = FTableRow { ID };
		row = &table.Rows.Add(ID, r);
	}

	return *row;
}

const FTableField* TableDatabase::GetField(const FString& tableName, uint32 columnId, uint32 id)
{
	const FTableRow& row = GetRow(tableName, id);

	for (auto& field : row.Fields)
	{
		if (field.Key.ColumnID != columnId)
			continue;

		return &field.Value;
	}

	return nullptr;
}

const FTableField* TableDatabase::GetField(const FString& tableName, const FString& columnName, uint32 id)
{
	const FTableRow& row = GetRow(tableName, id);

	for (auto& field : row.Fields)
	{
		if (field.Key.ColumnName != columnName)
			continue;

		return &field.Value;
	}

	return nullptr;
}

TPair<uint32, const FTableRow*> TableDatabase::FindLink(const FString& tableName, const FString& columnName, uint32 ID)
{
	const FTableData& table = GetTable(tableName);

	for (auto& row : table.Rows)
	{
		for (auto& field : row.Value.Fields)
		{
			if (!(field.Key == columnName))
				continue;

			if (field.Value.Size != 0)
			{
				double d = UINT_MAX;
				memcpy(&d, field.Value.Data.Get(), FMath::Min(sizeof(double), static_cast<size_t>(field.Value.Size)));

				if (static_cast<uint32>(d) == ID)
					return TPair<uint32, const FTableRow*>(row.Key, &row.Value);
			}
		}
	}

	return TPair<uint32, const FTableRow*>(UINT_MAX, nullptr);
}

TArray<TPair<uint32, const FTableRow*>> TableDatabase::FindLinks(const FString& tableName, const FString& columnName,
	uint32 ID)
{
	TArray<TPair<uint32, const FTableRow*>> Result;

	const FTableData& table = GetTable(tableName);

	for (auto& row : table.Rows)
	{
		for (auto& field : row.Value.Fields)
		{
			if (!(field.Key == columnName))
				continue;

			if (field.Value.Size != 0)
			{
				double d = UINT_MAX;
				memcpy(&d, field.Value.Data.Get(), FMath::Min(sizeof(double), static_cast<size_t>(field.Value.Size)));

				if (static_cast<uint32>(d) == ID)
					Result.Add(TPair<uint32, const FTableRow*>(row.Key, &row.Value));
			}
		}
	}

	return Result;
}

void TableDatabase::RemoveCache()
{
	Data.Empty();
}

void TableDatabase::Refresh()
{
	// Clear out the data
	RemoveCache();
	// UpdateTime();

#if UE_EDITOR
	// Get existing database files
	const FDatabaseConfig config = Fetch();
	const auto assetDirectory = config.dataPath;

	TArray<FString> filePaths;
	filePaths.Empty();
	if (FPaths::DirectoryExists(assetDirectory))
	{
		const FString path = assetDirectory + "/*.json";
		FFileManagerGeneric::Get().FindFiles(filePaths, *path, true, false);
		for (int i = 0; i < filePaths.Num(); i++)
		{
			filePaths[i] = assetDirectory + '/' + filePaths[i];
			const auto filePath = filePaths[i];
			GLog->Log(ELogVerbosity::Display, filePaths[i]);

			if(FPaths::FileExists(filePath)) {
				// Retrieve data from existing file, if it exists
				LoadExisting(filePath);
			}
		}
	}

#else
	// Fetch existing data from the addressable list.
	string fileName = Path.GetFileNameWithoutExtension(filePath);
	HelperClass.GetFileFromAddressable<TableSO>(fileName).Completed += handle => {
		if (handle.Result == null) return;

		// Retrieve data from existing file, if it exists
		handle.Result.Refresh();
		Data.Add(handle.Metadata.title, handle);
	};
#endif
}

void TableDatabase::AddTable(const TSharedPtr<FJsonObject>& item, const FTableMetaData& metadata)
{
	TableBinary* table = Data.Find(metadata.title);

	if(!table) {
		// load it from the addressable
		// if null again
		TableBinary bin = TableBinary(metadata);
		table = &Data.Add({ metadata.title, bin });
	}

	table->Import(item);
}

void TableDatabase::LoadExisting(const FString& filePath)
{
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *filePath);

	// Create a pointer to hold the json serialized data
	TSharedPtr<FJsonValue> JsonValue;

	// Create a reader pointer to read the json data
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	// Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid()) {
		const TSharedPtr<FJsonObject>& JsonObject = JsonValue->AsObject();

		FTableMetaData tableMetadata;
		if(FJsonObjectConverter::JsonObjectToUStruct(JsonObject->GetObjectField("metadata").ToSharedRef(), &tableMetadata))
		{
			TableBinary* table = Data.Find(tableMetadata.title);
			if(!table) {
				// load it from the addressable
				// if null again
				table = &Data.Add({ tableMetadata.title, TableBinary() });
			}
			table->Refresh(JsonObject);
		}
	}
}

