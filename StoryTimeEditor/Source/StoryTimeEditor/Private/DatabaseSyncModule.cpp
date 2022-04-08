// Fill out your copyright notice in the Description page of Project Settings.


#include "DatabaseSyncModule.h"

#include "Misc/FileHelper.h"
#include "Misc/MessageDialog.h"

#include "Interfaces/IPluginManager.h"
#include "Framework/Notifications/NotificationManager.h"

#include "Runtime/Online/HTTP/Public/Http.h"

#include "Components/Base/DataAssets/TableDA.h"
#include "Database/TableDatabase.h"

#include "JsonObjectConverter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#define LOCTEXT_NAMESPACE "FStoryTimeEditorModule"

DatabaseSyncModule* DatabaseSyncModule::s_Instance = nullptr;
const std::string DatabaseSyncModule::s_GroupName = std::string("JSON_data");
FString DatabaseSyncModule::s_DataPath = FString();
FString DatabaseSyncModule::FirebaseAppFile = FString();

DatabaseSyncModule& DatabaseSyncModule::Get()
{
	if(s_Instance == nullptr) {
		s_Instance = new DatabaseSyncModule();
	}

	return *s_Instance;
}

DatabaseSyncModule::DatabaseSyncModule()
{
	GLog->Log(ELogVerbosity::Display, TEXT("Starting module"));

	Http = &FHttpModule::Get();

	s_DataPath = FPaths::ConvertRelativePathToFull(IPluginManager::Get().FindPlugin(TEXT("StoryTimeEditor"))->GetBaseDir());

	// TODO this file should already exist to override it in order projects.
	FirebaseAppFile = s_DataPath / TEXT("Resources/config/firebase-storytime-app.json");

	// First retrieve the file
	RetrieveAppFile();

	// load the files that we already have locally.
	TableDatabase::Get().Refresh();

	CheckLogin([=] { });
}

DatabaseSyncModule::~DatabaseSyncModule()
{
}

/// <summary>
/// Request data from Firebase.
/// This methods allows the user to fetch all tables or a single table
/// from the Firebase database
/// </summary>
/// <param name="tableID"></param>
void DatabaseSyncModule::RequestTableUpdate(const FString& tableID)
{
	// Wait few seconds before we let the user click again.
	if (!bCanFetch || FDateTime::Now() - LastChecked < 50000000)
	{
		FText title_text = FText::FromString("Please wait");

		FMessageDialog::Open(
			EAppMsgType::Ok,
			FText::FromString("We are already processing data from server!"),
			&title_text
		);

		return;
	}

	bCanFetch = false;
	LastChecked = FDateTime::Now();

	// Fetch notification
	{
		FNotificationInfo Info(LOCTEXT("FetchDatabase", "Fetching database..."));
		// Info.Image = FEditorStyle::GetBrush(TEXT("LevelEditor.RecompileGameCode"));
		Info.FadeInDuration = 0.1f;
		Info.FadeOutDuration = 0.5f;
		Info.ExpireDuration = 1.5f;
		Info.bUseThrobber = false;
		Info.bUseSuccessFailIcons = false;
		Info.bUseLargeFont = true;
		Info.bFireAndForget = false;
		Info.bAllowThrottleWhenFrameRateIsLow = false;

		FetchNotification = FSlateNotificationManager::Get().AddNotification(Info);
	}

	// This is the url on which to process the request
	// TODO make timestamp work
	// string page = string.IsNullOrEmpty(table) ? "?tstamp=" : "tables/" + table + "?tstamp=";
	// check if we are logged in and then fetch data
	CheckLogin([=]
	{
		// TODO add regular expression for the product id in order to prevent errors.
		const FDatabaseConfig configFile = Fetch().Get({});

		GLog->Log(ELogVerbosity::Display, "Attempting to fetch data from" + configFile.databaseURL + "firebase/projects/" + configFile.projectID + "/tables" + tableID + "?tstamp=" + FString::FromInt(iLastTimeStamp));

		const TSharedRef<IHttpRequest> request = Http->CreateRequest();
		request->SetURL(configFile.databaseURL + "firebase/projects/" + configFile.projectID + "/tables" + tableID + "?tstamp=" + FString::FromInt(iLastTimeStamp));
		request->SetTimeout(60);
		request->SetVerb("GET");
		request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
		request->SetHeader("Content-Type", "application/json; charset=utf-8");
		request->SetHeader("Authorization", "Bearer " + DatabaseToken.id_token);
		request->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			OnResponseReceived(Request, Response, bWasSuccessful, tableID);
		});
		request->ProcessRequest();
	});
}

void DatabaseSyncModule::RetrieveAppFile()
{
	if(!FPaths::FileExists(FirebaseAppFile)) {
		GLog->Log(ELogVerbosity::Display, TEXT("Creating Firebase-storytime-app.json"));
		return;
	}

	// Json converted to FString
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *FirebaseAppFile);

	// Displaying the json in a string format inside the output log
	GLog->Log(FString("Json String: ") / JsonString);

	if(!FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &DatabaseToken,0,0)) {
		GLog->Log(ELogVerbosity::Error, "Couldn't deserialize token!");
	}
}

TOptional<FDatabaseConfig> DatabaseSyncModule::Fetch(bool bForceReload)
{
	// Load it.
	if ( !Config.IsSet() || bForceReload)
	{
		FString Json;
		const auto configFilePath = s_DataPath / TEXT("Resources/config/config.json");
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

	return Config;
}

void DatabaseSyncModule::Refresh(const std::function<void()>& callback)
{
	const FDatabaseConfig configFile = Fetch().Get({});

	if (configFile.email.IsEmpty() || configFile.password.IsEmpty())
	{
		GLog->Log(ELogVerbosity::Error, "Database configuration are not configured!");
		return;
	}

	const TSharedRef<IHttpRequest> request = Http->CreateRequest();
	request->SetURL(configFile.databaseURL / "refresh");
	request->SetTimeout(60);
	request->SetVerb("GET");
	request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", "application/json; charset=utf-8");
	request->SetHeader("Authorization", "Bearer " + DatabaseToken.id_token);
	request->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		// Handle result
		const FString str = Response->GetContentAsString();

		if (!bWasSuccessful || Response->GetResponseCode() == 400 || Response->GetResponseCode() == 401 || Response->GetResponseCode() == 500)
		{
			GLog->Log(ELogVerbosity::Error, "Could not refresh token!");
			if(FetchNotification) {
				FetchNotification->SetCompletionState(SNotificationItem::CS_Fail);
				FetchNotification->ExpireAndFadeout();
			}
			bCanFetch = true;
			return;
		}

		if (FJsonObjectConverter::JsonObjectStringToUStruct(str, &DatabaseToken, 0, 0) ) {
			// add the 3600 seconds for the next iteration
			const FTimespan exp = FTimespan(FDateTime::Now().GetTicks());
			exp.FromSeconds(exp.GetSeconds() + DatabaseToken.expires_in);
			DatabaseToken.expire_time += exp.GetTicks();

			FString streamDatabaseTokenString;
			FJsonObjectConverter::UStructToJsonObjectString(DatabaseToken, streamDatabaseTokenString, 0, 0);
			FFileHelper::SaveStringToFile(streamDatabaseTokenString, *FirebaseAppFile);
		}
		else {
			GLog->Log(ELogVerbosity::Error, "Couldn't deserialize config file!");
		}

		callback();
	});
	request->ProcessRequest();
}


void DatabaseSyncModule::CheckLogin(const std::function<void()>& callback)
{
	// See if the token is there, if not login
	if (DatabaseToken.id_token.IsEmpty())
	{
		// Login if everything is empty
		Login(callback);
		return;
	}

	// see if the token is expired
	// if the time is lower that means we are expired
	// Debug.Log(DATABASE_TOKEN.expire_time < DateTime.Now.Ticks);
	if (DatabaseToken.expire_time < FDateTime::Now().GetTicks() )
	{
		const FDatabaseConfig configFile = Fetch().Get({});

		const TSharedRef<IHttpRequest> request = Http->CreateRequest();
		request->SetURL(configFile.databaseURL / "me");
		request->SetTimeout(60);
		request->SetVerb("GET");
		request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
		request->SetHeader("Content-Type", "application/json; charset=utf-8");
		request->SetHeader("Authorization", "Bearer " + DatabaseToken.id_token);
		request->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!bWasSuccessful || Response->GetResponseCode() == 401 || Response->GetResponseCode() == 500)
			{
				// Log the user in again
				Login(callback);
				return;
			}

			// go to callback directly
			callback();
		});
		request->ProcessRequest();
		return;
	}

	// We can call the callback immediately
	callback();
}

void DatabaseSyncModule::Login(const std::function<void()>& callback)
{
	const FDatabaseConfig configFile = Fetch().Get({});

	if (configFile.email.IsEmpty() || configFile.password.IsEmpty())
	{
		GLog->Log(ELogVerbosity::Error, "Database configuration are not configured!");
		if(FetchNotification) {
			FetchNotification->SetCompletionState(SNotificationItem::CS_Fail);
			FetchNotification->ExpireAndFadeout();
		}
		bCanFetch = true;
		return;
	}

	GLog->Log(ELogVerbosity::Display, "Login in to retrieve token");

	const TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("email", configFile.email);
	RequestObj->SetStringField("password", configFile.password);

	FString RequestBody;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	const FString URL = configFile.databaseURL / "auth/authenticate";
	const TSharedRef<IHttpRequest> request = Http->CreateRequest();
	request->SetURL(URL);
	request->SetTimeout(60);
	request->SetVerb("POST");
	request->SetContentAsString(RequestBody);
	request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", "application/json");
	request->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		if (!bWasSuccessful || Response->GetResponseCode() == 401 || Response->GetResponseCode() == 500) {
			GLog->Log(ELogVerbosity::Error, "Error authenticating the user!");
			if(FetchNotification) {
				FetchNotification->SetCompletionState(SNotificationItem::CS_Fail);
				FetchNotification->ExpireAndFadeout();
			}
			bCanFetch = true;
			return;
		}

		// Handle result
		const FString str = Response->GetContentAsString();

		if (FJsonObjectConverter::JsonObjectStringToUStruct(str, &DatabaseToken, 0, 0) ) {
			// add the 3600 seconds for the next iteration
			const FTimespan exp = FTimespan(FDateTime::Now().GetTicks());
			exp.FromSeconds(exp.GetSeconds() + DatabaseToken.expires_in);
			DatabaseToken.expire_time += exp.GetTicks();

			FString streamDatabaseTokenString;
			FJsonObjectConverter::UStructToJsonObjectString(DatabaseToken, streamDatabaseTokenString, 0, 0);
			FFileHelper::SaveStringToFile(streamDatabaseTokenString, *FirebaseAppFile);
		}
		else {
			GLog->Log(ELogVerbosity::Error, "Couldn't deserialize config file!");
		}

		callback();
	});
	request->ProcessRequest();
}

void DatabaseSyncModule::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, const FString& tableID)
{
	FetchNotification->SetCompletionState(bWasSuccessful ? SNotificationItem::CS_Success : SNotificationItem::CS_Fail);
	FetchNotification->ExpireAndFadeout();
	if (!bWasSuccessful || Response->GetResponseCode() == 401 || Response->GetResponseCode() == 500)
	{
		FNotificationInfo Info(FText::FromString("Database error:\n\" Could not retrieve data from database!"));

		Info.FadeInDuration = 0.1f;
		Info.FadeOutDuration = 0.5f;
		Info.ExpireDuration = 5.0f;
		Info.bUseThrobber = false;
		Info.bUseSuccessFailIcons = false;
		Info.bUseLargeFont = false;
		Info.bFireAndForget = true;
		Info.bAllowThrottleWhenFrameRateIsLow = false;

		const auto Notification = FSlateNotificationManager::Get().AddNotification(Info);
		Notification->SetCompletionState(SNotificationItem::CS_Fail);

		// GLog->Log(ELogVerbosity::Error, "Error: " + request.error);
		// GLog->Log(ELogVerbosity::Error, "Error: " + request.downloadHandler.text);
		bCanFetch = true;
		return;
	}

	// Get latest check timestamp from file
	FString path = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectContentDir());
	path.Append("Data/update.time");

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	IFileHandle* FileHandle = PlatformFile.OpenRead(*path);

	// When file can be opened
	if (FileHandle)
	{
		FileHandle->Read((uint8_t*)&iLastTimeStamp, sizeof(uint64_t));
		delete FileHandle;
	}
	else
	{
		iLastTimeStamp = 0;
	}

	// Handle result
	const FString string = Response->GetContentAsString();

	// Create a pointer to hold the json serialized data
	TSharedPtr<FJsonValue> JsonValue;

	// Create a reader pointer to read the json data
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(string);

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
	{
		// Create a pointer to hold the json serialized data
		if (tableID != "")
		{
			const auto JsonObject = JsonValue->AsObject();

			if (!JsonObject->HasField("id") || !JsonObject->HasField("projectID") || !JsonObject->HasField("metadata"))
				throw std::invalid_argument("Can't make Table from JSON file");

			// Get the TableSO from the TableDatabase.
			// TableMetaData tableMetadata = jsonToken["metadata"].ToObject<TableMetaData>();
			// TableDatabase.Get.AddTable(jsonToken, tableMetadata);
		}
		else
		{
			auto jsonArray = JsonValue->AsArray();

			// now we can get the values from json of any attribute.
			for (auto it = jsonArray.begin(); it != jsonArray.end(); ++it) {
				auto item = (*it)->AsObject();

				if (!item->HasField("id") || !item->HasField("projectID") || !item->HasField("metadata"))
					throw std::invalid_argument("Can't make Table from JSON file");

				FTableMetaData tableMetadata;
				if(FJsonObjectConverter::JsonObjectToUStruct(item->GetObjectField("metadata").ToSharedRef(), &tableMetadata))
				{
					// Export to addressable is handle in the Table itself --> SOLID PATTERN
					TableDatabase::Get().AddTable(item, tableMetadata);
				}
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
