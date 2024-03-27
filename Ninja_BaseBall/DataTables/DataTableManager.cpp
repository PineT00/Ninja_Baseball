#include "pch.h"
#include "BossTable.h"

DataTableManager::DataTableManager()
{
}

DataTableManager::~DataTableManager()
{
}

void DataTableManager::Init()
{
	Release();
	
	DataTable* bossTable = new BossTable(DataTable::Types::Boss);
	bossTable->Load();
	tables.insert({ DataTable::Types::Boss, bossTable });
}

void DataTableManager::Release()
{
	for (auto table : tables)
	{
		delete table.second;
	}
	tables.clear();
}
