// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Notifications/SNotificationList.h"

#include <string>

#include "Config/DatabaseConfig.h"
#include "Interfaces/IHttpRequest.h"

#include "DatabaseSyncModule.generated.h"

USTRUCT(BlueprintType)
struct FDatabaseToken
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Database API")
	FString token_type;

	UPROPERTY(BlueprintReadOnly, Category="Database API")
	FString access_token;

	UPROPERTY(BlueprintReadOnly, Category="Database API")
	FString id_token;

	UPROPERTY(BlueprintReadOnly, Category="Database API")
	FString refresh_token;

	UPROPERTY(BlueprintReadOnly, Category="Database API")
	int32 expires_in = -1;

	int64_t expire_time;
};

/**
 *
 */
class FHttpModule;
class DatabaseSyncModule
{
public:
	static DatabaseSyncModule& Get();

	DatabaseSyncModule();
	~DatabaseSyncModule();

	// TODO create global Unreal event.

	void RequestTableUpdate(const FString& tableID = "");
protected:

	void RetrieveAppFile();
	TOptional<FDatabaseConfig> Fetch(bool bForceReload = true);
	void Refresh(const std::function<void()>& callback);
	void CheckLogin(const std::function<void()>& callback);

private:
	static DatabaseSyncModule* s_Instance;
	static const std::string s_GroupName;
	static FString s_DataPath;
	static FString FirebaseAppFile;

	// Database information

	void Login(const std::function<void()>& callback);
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, const FString& tableID = "");

	TOptional<FDatabaseConfig> Config;
	FDatabaseToken DatabaseToken;

	// The last time we sync data from the database.
	int64_t iLastTimeStamp;

	// Ability to fetch from the database again.
	FDateTime LastChecked;
	bool bCanFetch = true;

	TSharedPtr<SNotificationItem> FetchNotification;
	FHttpModule* Http = nullptr;
};
