// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DatabaseConfig.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FDatabaseConfig
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category="Database API")
	FString databaseURL;

	UPROPERTY(BlueprintReadOnly, Category="Database API")
	FString projectID;

	UPROPERTY(BlueprintReadOnly, Category="Database API")
	FString email;

	UPROPERTY(BlueprintReadOnly, Category="Database API")
	FString password;

	UPROPERTY(BlueprintReadOnly, Category="Database API")
	FString dataPath;

	FDatabaseConfig()
		: databaseURL("")
		, projectID("")
		, email("")
		, password("")
		, dataPath("")
	{
	}
};
