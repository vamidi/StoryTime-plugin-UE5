#include "Database/Binary/TableBinary.h"

#include "Config/DatabaseConfig.h"
#include "Database/TableDatabase.h"

#include "Misc/FileHelper.h"

#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "TableBinary"

#define ADD_NOTIFICATION FNotificationInfo Info(DialogText); \
Info.FadeInDuration = 0.1f; \
Info.FadeOutDuration = 0.5f; \
Info.ExpireDuration = 5.0f; \
Info.bUseThrobber = false; \
Info.bUseSuccessFailIcons = false; \
Info.bUseLargeFont = false; \
Info.bFireAndForget = true; \
Info.bAllowThrottleWhenFrameRateIsLow = false; \
auto Notification = FSlateNotificationManager::Get().AddNotification(Info); \

uint32_t TableBinary::GetColumnID(FString columnName) const
{
	const std::string cName = TCHAR_TO_UTF8(*columnName);

	for (uint32_t j = 0; j < m_Ujson.ColumnCount; ++j)
	{
		std::string sColumnName = TCHAR_TO_UTF8(*m_Ujson.JsonColumnEntries[j].Name);
		if (strncmp(sColumnName.c_str(), cName.c_str(), cName.length()) == 0)
			return j;
	}

	return UINT_MAX;
}

FString TableBinary::GetColumnName(uint32 columnId)
{
	if (columnId > m_Ujson.ColumnCount)
		return "";

	return m_Ujson.JsonColumnEntries[columnId].Name;
}

void TableBinary::Refresh()
{
#if UE_EDITOR
	// Retrieve data from existing file, if it exists
	m_Data.Empty();
	const TSharedPtr<FJsonObject>& token = GetTableData();
	ParseData(token);

#else
	// Load the data from the json string we already parsed at the beginning
	var token = TableBinary.GetTableData(jsonData);
	Binary.Refresh(token, ref Rows);

	// clear out existing data
	_data.Clear();
	var token = GetTableData(stream);

#endif
	// Debug.Log(this);
}

void TableBinary::Refresh(const TSharedPtr<FJsonObject>& stream)
{
#if UE_EDITOR
	// Retrieve data from existing file, if it exists
	m_Data.Empty();
	ParseData(stream);
#else
	// Load the data from the json string we already parsed at the beginning
	var token = TableBinary.GetTableData(jsonData);
	Binary.Refresh(token, ref Rows);

	// clear out existing data
	_data.Clear();
	var token = GetTableData(stream);

#endif
	// Debug.Log(this);
}

FString TableBinary::HandleJsonObject(const TSharedPtr<FJsonObject>& object)
{
	// see if we are dealing with a language file
	FString OutputString;
	if(object->HasField(m_LanguageToCheck)) {
		const TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

		JsonObject->SetStringField(m_LanguageToCheck, object->GetStringField(m_LanguageToCheck));

		TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

		return OutputString;
	}

	return OutputString;
}

TableBinary::TableBinary(const FTableMetaData& metadata)
{
	Metadata = metadata;
	// Retrieve data from existing file, if it exists
	Refresh();
}

/// <summary>
/// Import the JSON data and read from it.
/// </summary>
/// <param name="jsonTableData"></param>
void TableBinary::Import(const TSharedPtr<FJsonObject>& jsonTableData)
{
	m_bListUpdated = true;

	// Get highest ID
	uint32_t highestID = 0;

	if (!jsonTableData->HasField("id") || !jsonTableData->HasField("data") || !jsonTableData->HasField("metadata"))
		throw std::invalid_argument("JSON does not contains the property id, data or metadata");

	// if (m_ConfigFile == null)
	// throw new ArgumentException("Database Config file could not be found");

	// The key-value data
	TSharedPtr<FJsonObject> entries = jsonTableData->GetObjectField("data");

	// The metadata from the json stream
	TSharedPtr<FJsonObject> metaData = jsonTableData->GetObjectField("metadata");

	// {"Sellable":true,"created_at":1594979553,"deleted":false,"effectPrimaryValue":0,"effectTypeId":0,"name":"","sellValue":0,"updated_at":1594979553}

	// Check change in column
	// _UJSON.Entries = new List<JSONEntry>((int)_UJSON.EntityCount);

	TArray<TSharedPtr<FJsonValue>> Children;

	// Set data
	for (auto row = entries->Values.begin(); row != entries->Values.end(); ++row) {
		if(row == entries->Values.begin())
			m_Ujson.ColumnCount = row.Value()->AsObject()->Values.Num();

		highestID = std::max(highestID, static_cast<uint32_t>(FCString::Atoi(*row.Key())));
	}

	// set the count of the amount of entities in the json
	m_Ujson.EntityCount = std::max(
		m_Ujson.EntityCount,
		std::max(static_cast<uint32_t>(entries->Values.Num()), highestID + 1)
	);

	m_Data.SetNum(m_Ujson.EntityCount * m_Ujson.ColumnCount);

	int i = 0;
	for (auto rows = entries->Values.begin(); rows != entries->Values.end(); ++rows)
	{
		auto row = rows->Value->AsObject();
		FTableRow tblRow = FTableRow { static_cast<uint32_t>(i) };

		int j = 0;
		for (auto entity = row->Values.begin(); entity != row->Values.end(); ++entity)
		{
			m_Data[j].Data.Reset();

			uint32_t length;
			void* data;

			FString strData = entity.Value()->AsString();
			double dData = entity.Value()->AsNumber();
			bool bData = entity.Value()->AsBool();
			std::string sData;

			switch (entity.Value()->Type)
			{
			case EJson::Boolean:
				length = sizeof(uint8_t);
				data = &bData;
				// Debug.Log("Bool");
				break;
			case EJson::Number:
				length = sizeof(double);
				data = &dData;
				// Debug.Log("Number");
				break;
			default:
			case EJson::String:
				sData = std::string(TCHAR_TO_UTF8(*strData));
				length = sData.length();
				data = const_cast<char*>(sData.c_str());
				// Debug.Log("String");
				break;
			case EJson::Null:
				length = 0;
				data = nullptr;
			case EJson::Object:
				sData = std::string(TCHAR_TO_UTF8(*strData));
				sData.erase(std::remove(sData.begin(), sData.end(), '\t'), sData.end());
				sData.erase(std::remove(sData.begin(), sData.end(), '\n'), sData.end());
				// sData.erase(std::remove(sData.begin(), sData.end(), '\r'), sData.end());

				length = sData.length();

				data = const_cast<char*>(sData.c_str());
				break;
			}

			FTableRowInfo Info = FTableRowInfo { TCHAR_TO_UTF8(*entity.Key()), static_cast<uint32_t>(j) };

			m_Data[j].Size = length;

			if(length != 0) {
				m_Data[j].Data = MakeShareable(new uint8_t[m_Data[j].Size]);
				memcpy(m_Data[j].Data.Get(), data, m_Data[j].Size);
			}

			// Set the amount of columns existing in all the entities
			JsonEntry entry = { entity.Value()->Type, TCHAR_TO_UTF8(*entity.Key()) };
			m_Ujson.JsonColumnEntries.push_back(entry);

			FTableField Entry;
			Entry.Size = length;
			Entry.Data = m_Data[j].Data;

			// From there we need store the entries
			tblRow.Fields.Add(Info, Entry);
			j++;
		}

		// Add the row
		TableData.Rows.Add(i, tblRow);

		// create a Json value object to hold your Json object
		TSharedRef<FJsonValueObject> JsonValue = MakeShareable( new FJsonValueObject(row));
		Children.Add(JsonValue);
		i++;
	}

	const TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("id", jsonTableData->GetStringField("id"));
	RequestObj->SetObjectField("metadata", metaData);
	RequestObj->SetArrayField("data", Children);

	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&m_JsonData);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Export();
}

void TableBinary::Export() const
{
	const FDatabaseConfig config = TableDatabase::Fetch();

	FString path = config.dataPath;
	path.Append("/" + Metadata.title);
	path.AppendChar('.');
	path.Append("json");

	// Write to file
	FFileHelper::SaveStringToFile(m_JsonData, *path);

	const FText DialogText = FText::Format(
		LOCTEXT("Succefully saved table file", "Succesfully saved the table file to:\n\"{0}\"!"),
		FText::FromString(path)
	);

	ADD_NOTIFICATION
	Notification->SetCompletionState(SNotificationItem::CS_Success);
}

const TSharedPtr<FJsonObject>& TableBinary::GetTableData() const
{
	// Create a pointer to hold the json serialized data
	TSharedPtr<FJsonValue> JsonValue;

	// Create a reader pointer to read the json data
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(m_JsonData);

	//Deserialize the json data given Reader and the actual object to deserialize
	FJsonSerializer::Deserialize(Reader, JsonValue); // && JsonValue.IsValid();

	return JsonValue->AsObject();
}

/// <summary>
/// The table data is read from the JSON token and then inserted
/// in the table struct.
/// </summary>
/// <param name="tableData"></param>
/// <returns></returns>
void TableBinary::ParseData(const TSharedPtr<FJsonObject>& tableData)
{
	const TArray<TSharedPtr<FJsonValue>> entities = tableData->GetArrayField("data");

	// set the count of the amount of entities in the json
	m_Ujson.EntityCount = static_cast<uint32_t>(entities.Num());

	uint32_t i = 0;

	// Loop through all the entities
	for (const auto& el : entities) {
		const TSharedPtr<FJsonObject>& row = el->AsObject();

		FTableRow tblRow = FTableRow{ i };
		if (i == 0)
		{
			// Set the amount of columns existing in all the entities
			m_Ujson.ColumnCount = static_cast<uint32_t>(row->Values.Num());
			m_Data.SetNum(m_Ujson.EntityCount * m_Ujson.ColumnCount);

			// We need to story all the data in one large array
			// _ujson.JsonColumnEntries.Capacity = (int)(_ujson.ColumnCount * _ujson.EntityCount);
		}

		uint32_t j = 0;
		for (auto entity = row->Values.begin(); entity != row->Values.end(); ++entity)
		{
			// From there we need store the entries
			const uint32_t id = i * m_Ujson.ColumnCount + j;
			m_Data[id].Data.Reset();

			FTableField Field = {};

			uint32_t length;
			void* data;

			FString strData = entity.Value()->AsString();
			double dData = entity.Value()->AsNumber();
			bool bData = entity.Value()->AsBool();
			std::string sData;

			switch (entity.Value()->Type)
			{
			case EJson::Boolean:
				length = sizeof(uint8_t);
				data = &bData;
				break;
			case EJson::Number:
				length = sizeof(double);
				data = &dData;
				break;
			default:
			case EJson::String:
				sData = std::string(TCHAR_TO_UTF8(*strData));
				length = sData.length();
				data = const_cast<char*>(sData.c_str());
				break;
			case EJson::Object:
				FString OutputString;
				TSharedRef<TJsonWriter<WIDECHAR, TCondensedJsonPrintPolicy<WIDECHAR>>> Writer = TJsonWriterFactory<WIDECHAR, TCondensedJsonPrintPolicy<WIDECHAR>>::Create(&OutputString);
				FJsonSerializer::Serialize(entity.Value()->AsObject().ToSharedRef(), Writer);

				length = OutputString.Len();
				auto twoHundredAnsi = StringCast<ANSICHAR>(*OutputString);
				data = const_cast<char*>(twoHundredAnsi.Get());

				break;
			}


			// Add the field to the table row as well.
			FTableRowInfo Info = FTableRowInfo { TCHAR_TO_UTF8(*entity.Key()), id };

			m_Data[id].Size = length;

			// Set the amount of columns existing in all the entities
			JsonEntry entry = { entity.Value()->Type, TCHAR_TO_UTF8(*entity.Key()) };
			m_Ujson.JsonColumnEntries.push_back(entry);

			if(length != 0) {
				m_Data[id].Data = MakeShareable(new uint8_t[m_Data[id].Size]);
				memcpy(m_Data[id].Data.Get(), data, m_Data[id].Size);
			}

			FTableField Entry;
			Entry.Size = length;
			Entry.Data = m_Data[id].Data;

			// From there we need store the entries
			tblRow.Fields.Add(Info, Entry);
			j++;
		};

		if (TableData.Rows.Contains(i))
			TableData.Rows[i] = tblRow;
		else
			TableData.Rows.Add(i, tblRow);
		i++;
	}
}

/// <summary>
/// Generate a list with columns from the JSON data
/// </summary>
/// <param name="columnToShow"></param>
/// <param name="isJsonObj"></param>
/// <returns></returns>
const TMap<uint32, FString>& TableBinary::Populate(const FString& columnToShow)
{
	// isJsonObj = false;

	const uint32_t uiColumnID = GetColumnID(columnToShow);

	if (m_ShownColumn == columnToShow && !m_bListUpdated)
	{
		const int32 idx = static_cast<int32>(uiColumnID);
		if (idx > 0 && idx < m_Data.Num())
		{
			const auto data = m_Data[idx].Data.Get();
			// isJsonObj = data is JObject;
		}

		return _list;
	}

	m_bListUpdated = false;
	m_ShownColumn = columnToShow;

	for (uint32_t i = 0; i < m_Ujson.EntityCount; ++i)
	{
		// if (!_UJSON.DSCT.Pointers[i])
		// {
		//     _list.Remove(i);
		//     continue;
		// }

		FString d = "";
		if (uiColumnID == UINT_MAX)
			d = FString::FromInt(i);
		else
		{
			const TSharedPtr<uint8_t> data = m_Data[(i * m_Ujson.ColumnCount + uiColumnID)].Data;
			d += "[" + FString::FromInt(i) + "] ";

			FString Result;
			Result.Append(UTF8_TO_TCHAR(data.Get()), m_Data[(i * m_Ujson.ColumnCount + uiColumnID)].Size);

			// Create a pointer to hold the json serialized data
			TSharedPtr<FJsonValue> JsonValue;

			// Create a reader pointer to read the json data
			const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Result);

			// Deserialize the json data given Reader and the actual object to deserialize
			if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid()) {
				const TSharedPtr<FJsonObject>& JsonObject = JsonValue->AsObject();

				// isJsonObj = true;
				d += JsonObject->GetStringField(m_LanguageToCheck);
			} else {
				d = Result;
			}
		}

		// override if already exists
		_list.Add(i, d);
	}

	return _list;
}

/// <summary>
/// Populate a list with a link to another table or a link within the same table.
/// </summary>
/// <param name="columnToShow"></param>
/// <param name="linkedColumn"></param>
/// <param name="linkedId"></param>
/// <param name="isJObject"></param>
/// <param name="otherTable"></param>
/// <returns></returns>
TMap<uint32, FString> TableBinary::PopulateWithLink(const FString& columnToShow, const FString& linkedColumn, uint32 linkedId,
	/* bool& isJObject, */const FString& otherTable)
{
	// isJObject = false;
	// Find the other binary if we need to show the string that is inside that table.
	// If not that means the linked column is in the same table.
	const TableBinary* tableBinary = !otherTable.IsEmpty() ? &TableDatabase::Get().GetBinary(otherTable) : this;

	// Populate the <number, string value>
	TMap<uint32, FString> NewList = Populate(columnToShow/*, isJObject*/);
	// Get the index of the linked column
	const uint32 uiLinkedID = linkedColumn != "" ? GetColumnID(linkedColumn) : UINT_MAX;

	// Debug.Log($"otherTable: {otherTable}");
	// Debug.Log($"linkedColumn: {linkedColumn}");
	// Debug.Log($"linkId: {linkedId}");
	// Debug.Log($"index linkedID: {uiLinkedID}");

	// Remove if not linked
	if (uiLinkedID != UINT_MAX && linkedColumn != "")
	{
		TArray<uint32> Keys;
		NewList.GetKeys(Keys);

		for (const uint32 Key : Keys) {
			if(const FTableField* CurrentField = TableDatabase::Get().GetField(Metadata.title, uiLinkedID, Key)) {
				double id;
				memcpy(&id, CurrentField->Data.Get(), CurrentField->Size);

				if (static_cast<uint32>(id) != linkedId) {
					NewList.Remove(Key);
				}

				/*
				const FTableField* Otherfield = TableDatabase::Get().GetField(tableBinary.Metadata.title, columnToShow, id);
				string d = "";
				if (otherfield.Data is JObject)
				{
					d += $"[{key}] {otherfield.Data["en"]}";
				}
				else
					d += $"[{key}] {otherfield.Data}";

				newList[key] = d;
				*/
			}
		}

		NewList.Compact();
	}

	return NewList;
}

#undef LOCTEXT_NAMESPACE
