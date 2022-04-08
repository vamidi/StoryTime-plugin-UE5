// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Database/Binary/TableRow.h"
#include "TableDA.generated.h"

struct FTableData;

USTRUCT(BlueprintType)
struct FTableMetaData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	FString title;

	UPROPERTY(BlueprintReadOnly)
	FString description;

	long created_at = 0;

	bool deleted = false;

	uint32_t lastUID = 0;

	// public bool private = false;

	long updated_at = 0;
};


/**
 *
 */
UCLASS(Abstract)
class STORYTIME_API UTableDA:  public UPrimaryDataAsset
{
	GENERATED_BODY()
	friend class FTableSelector;

public:
	UTableDA();
	UTableDA(const FString& name, const FString& dropdownColumn, const FString& linkedColumn = "",
			uint32 linkedId = UINT_MAX, const FString& linkedTable = "");

	void Init(const FString& name, const FString& dropdownColumn, const FString& linkedColumn, uint32 linkedId, const FString& linkedTable);
	void virtual Reset() {}

	// Setters
	void SetID(uint32 newID);

	// public getters
	// Getters
	UFUNCTION(BlueprintCallable)
	int32 TableID() const { return static_cast<int32>(ID); }

	uint32 GetID() const { return ID; }
	const FString& GetTableName() const& { return Name; }
	const FString& GetDropdownColumn() const& { return DropdownColumn; }
	const FString& GetLinkedColumn() const& { return LinkedColumn; }
	uint32 GetLinkedID() const { return LinkedID; }

	const FTableMetaData& GetMetadata() const& { return metadata; }
	const FTableData& GetBinary() const&;

	const FTableField* GetField(const FString& tblName, uint32 columnId, uint32 id);
	const FTableField* GetField(const FString& tblName, const FString& columnName, uint32 id);

	const FTableRow& GetRow(const FString& tblName, uint32 id);

	const FTableData& GetTable(const FString& tblName);

	TPair<uint32, const FTableRow*> FindLink(const FString& tblName, const FString& columnName, uint32 id);
	TArray<TPair<uint32, const FTableRow*>> FindLinks(const FString tblName, const FString& columnName, uint32 id);

	// static FPeriod ConvertRow(const FTableRow& Row, int32 ID);
protected:
	UFUNCTION()
	virtual void OnTableIDChanged();
	virtual void ConvertRow() { }

	void SetName(const FString& newName);
	void SetDropdownColumn(const FString& newDropdownColumn);
	void SetLinkedID(uint32 newLinkID);
	void SetLinkedColumn(const FString& newLinkedColumn);
	void SetLinkedTable(const FString& newLinkedTable);

	UPROPERTY(EditDefaultsOnly, Category = "Table Properties")
	uint32 ID = UINT_MAX;
public:
	TMap<uint32, FTableRow> Rows = TMap<uint32, FTableRow>();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Table Properties")
	uint32 LinkedID = UINT_MAX;

	UPROPERTY(VisibleAnywhere, Category = "Table Properties")
	FString Name = "";

	UPROPERTY()
	FString DropdownColumn;

	UPROPERTY(VisibleAnywhere, Category = "Table Properties")
	FString LinkedColumn;

	UPROPERTY(VisibleAnywhere, Category = "Table Properties")
	FString LinkedTable;

	UPROPERTY()
	bool DropdownDisabled = false;

	FTableMetaData metadata;
	TObjectPtr<FTableData> binary;
};

