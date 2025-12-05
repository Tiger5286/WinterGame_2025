#include "DataManager.h"
#include <fstream>
#include "Dxlib.h"

namespace
{
	const char kDataFileName[] = "SaveData.dat";
}

DataManager::DataManager():
	_saveData()
{
}

DataManager::~DataManager()
{
}

void DataManager::Save()
{
	FILE* fp = nullptr;
	auto err = fopen_s(&fp, kDataFileName, "wb");
	if (fp == nullptr) return;

	DataHeader header;
	header.signature[0] = 's';
	header.signature[1] = 'd';
	header.signature[2] = 'a';
	header.signature[3] = 't';
	header.version = 1.0f;
	header.size = sizeof(_saveData);

	fwrite(&header, sizeof(header), 1, fp);
	fwrite(&_saveData, sizeof(_saveData), 1, fp);
	fclose(fp);
}

void DataManager::Load()
{
	auto fileName = kDataFileName;
	int handle = FileRead_open(fileName);
	DataHeader header;
	FileRead_read(&header, sizeof(header), handle);
	FileRead_read(&_saveData, sizeof(_saveData), handle);

	FileRead_close(handle);
}
