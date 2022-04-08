#pragma once

#include "Ujson.h"
#include "Components/Base/DataAssets/TableDA.h"
#include "Database/Binary/TableData.h"

struct FTableField;

class STORYTIME_API TableBinary
{
public:
	friend class TableDatabase;

	TableBinary() { }
	TableBinary(const FTableMetaData& MetaData);

	// Gets a value from the binary file
	// static FTableField GetValue(const FString& tableName, uint32_t columnID, uint32_t entityID);

	// Gets a row from the binary file
	// static FTableRow GetRow(const FString& tableName, uint32_t entityID);

	// Gets entire table from the binary file
	const FTableData& GetTable() const { return TableData; }

	uint32_t GetColumnID(FString columnName) const;
	FString GetColumnName(uint32 columnId);

	const TMap<uint32, FString>& Populate(const FString& columnToShow/*, bool& isJsonObj*/);
	TMap<uint32, FString> PopulateWithLink(const FString& columnToShow, const FString& linkedColumn, uint32 linkedId,
			/*bool& isJObject,*/ const FString& otherTable = "");

protected:
	void Refresh();
	void Refresh(const TSharedPtr<FJsonObject>& stream);

	FString HandleJsonObject(const TSharedPtr<FJsonObject>& object);

private:
	void Import(const TSharedPtr<FJsonObject>& jsonTableData);
	void Export() const;

	const TSharedPtr<FJsonObject>& GetTableData() const;
	void ParseData(const TSharedPtr<FJsonObject>& tableData);

	FTableMetaData Metadata = {};
	FTableData TableData = {};

	/// <summary>
	/// This container contains all the data we need to get the right column
	/// </summary>
	UJSON m_Ujson = {};

	bool m_bListUpdated = false;
	FString m_ShownColumn;
	FString m_JsonData;
	FString m_LanguageToCheck = "en";

	TMap<uint32_t, FString> _list = TMap<uint32_t, FString>({
		{ UINT_MAX, "None" }
	});

	// the fields that are determined in this table.
	TArray<FTableField> m_Data = {};
};
