#pragma once

#include <vector>

struct JsonEntry {
	EJson Type;				// Column type
	FString Name;			// Column name
};

struct JsonEntity
    {
    	std::vector<JsonEntry> Columns;		// Json data entries
    };

struct UJSON
{
    uint64_t Modified;						// Last modified timestamp
    char Name[32];							// Table Name
    uint32_t ColumnCount = 0;				// Number of columns within the table
	uint32_t EntityCount = 0;				// Number of entities within the table

    std::vector<JsonEntry> JsonColumnEntries;		// Json data entries
};
