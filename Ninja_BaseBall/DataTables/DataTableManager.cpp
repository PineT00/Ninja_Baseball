#include "pch.h"

DataTableManager::DataTableManager()
{
}

DataTableManager::~DataTableManager()
{
}

void DataTableManager::Init()
{
	Release();
	
}

void DataTableManager::Release()
{
	for (auto table : tables)
	{
		delete table.second;
	}
	tables.clear();
}
