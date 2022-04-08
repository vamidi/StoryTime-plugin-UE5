// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Base/DataAssets/TableDA.h"

#include "Database/TableDatabase.h"

UTableDA::UTableDA(): UTableDA("", "") { }

UTableDA::UTableDA(const FString& name, const FString& dropdownColumn, const FString& linkedColumn, uint32_t linkedId,
	const FString& linkedTable): UPrimaryDataAsset()
{
	Init(name, dropdownColumn, linkedColumn, linkedId, linkedTable);
}

void UTableDA::Init(const FString& name, const FString& dropdownColumn, const FString& linkedColumn,
	uint32_t linkedId, const FString& linkedTable)
{
	Name = name;
	DropdownColumn = dropdownColumn;
	LinkedColumn = linkedColumn;
	LinkedID = linkedId;
	LinkedTable = linkedTable;
}

void UTableDA::SetID(uint32_t newID)
{
	ID = newID;
	OnTableIDChanged();
}

const FTableField* UTableDA::GetField(const FString& tblName, uint32 columnId, uint32 id)
{
	return TableDatabase::Get().GetField(tblName, columnId, id);
}

const FTableField* UTableDA::GetField(const FString& tblName, const FString& columnName, uint32 id)
{
	return TableDatabase::Get().GetField(tblName, columnName, id);
}

const FTableRow& UTableDA::GetRow(const FString& tblName, uint32 id)
{
	return TableDatabase::Get().GetRow(tblName, id);
}

const FTableData& UTableDA::GetTable(const FString& tblName)
{
	return TableDatabase::Get().GetTable(tblName);
}

TPair<uint32, const FTableRow*> UTableDA::FindLink(const FString& tblName, const FString& columnName, uint32 id)
{
	return TableDatabase::Get().FindLink(tblName, columnName, id);
}

TArray<TPair<uint32, const FTableRow*>> UTableDA::FindLinks(const FString tblName, const FString& columnName, uint32 id)
{
	return TableDatabase::Get().FindLinks(tblName, columnName, id);
}

void UTableDA::OnTableIDChanged()
{
	TableDatabase::Get().Refresh();
	if(ID != UINT_MAX)
		ConvertRow();
}

void UTableDA::SetName(const FString& newName)
{
	Name = newName;
}

void UTableDA::SetDropdownColumn(const FString& newDropdownColumn)
{
	DropdownColumn = newDropdownColumn;
}

void UTableDA::SetLinkedID(uint32_t newLinkedID)
{
	LinkedID = newLinkedID;
}

void UTableDA::SetLinkedColumn(const FString& newLinkedColumn)
{
	LinkedColumn = newLinkedColumn;
}

void UTableDA::SetLinkedTable(const FString& newLinkedTable)
{
	LinkedTable = newLinkedTable;
}

const FTableData& UTableDA::GetBinary() const&
{
	return *binary;
}

