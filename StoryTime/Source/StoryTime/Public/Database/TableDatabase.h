// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Binary/TableBinary.h"
#include "Config/DatabaseConfig.h"

struct FTableData;
struct FTableMetaData;
struct FTableRow;
struct FTableField;

class DatabaseSyncModule;

/**
 *
 */
class STORYTIME_API TableDatabase
{
public:
	friend DatabaseSyncModule;

	static TableDatabase& Get();
	static const FDatabaseConfig& Fetch();

protected:
	TableDatabase();

public:
	~TableDatabase();

	// Gets an entire table from memory
	const FTableData& GetTable(const FString& tableName);
	TableBinary& GetBinary(const FString& tableName);

	// Gets a row from memory
	const FTableRow& GetRow(const FString& tableName, uint32 id);

	// Gets a field from binary if non-existent, otherwise gets it from memory
	const FTableField* GetField(const FString& tableName, uint32 columnId, uint32 id);
	const FTableField* GetField(const FString& tableName, const FString& columnName, uint32 id);

	TPair<uint32, const FTableRow*> FindLink(const FString& tableName, const FString& columnName, uint32 id);
	TArray<TPair<uint32, const FTableRow*>> FindLinks(const FString& tableName, const FString& columnName, uint32 id);

	// Gets a binary file from table name
	// TableBinary& GetBinary(const FString& tableName);

	// UDialogueSettingsConfigDA FetchDialogueSetting() const;

	// Deletes all the data cached in memory
	void RemoveCache();

	// Reads from all the binaries, updates data and updates the editor
	void Refresh();

private:
	void AddTable(const TSharedPtr<FJsonObject>& item, const FTableMetaData& metadata);
	void LoadExisting(const FString&);
	// void UpdateTime();

	static TableDatabase* s_Instance;
	static TOptional<FDatabaseConfig> Config;

	// All the data (sorted per table) the application needs for reading
	TMap<FString, TableBinary> Data;
};

