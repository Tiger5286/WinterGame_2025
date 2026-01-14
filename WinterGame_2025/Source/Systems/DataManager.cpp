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
	// ファイルを開く
	FILE* fp = nullptr;
	auto err = fopen_s(&fp, kDataFileName, "wb");
	if (fp == nullptr) return;

	// 書き込み用のヘッダを用意
	SaveDataHeader header;
	header.signature[0] = 's';
	header.signature[1] = 'd';
	header.signature[2] = 'a';
	header.signature[3] = 't';
	header.version = 1.0f;
	header.size = sizeof(_saveData);

	// ヘッダの書き込み
	fwrite(&header, sizeof(header), 1, fp);
	// セーブデータ本体の書き込み
	fwrite(&_saveData, sizeof(_saveData), 1, fp);
	// ファイルを閉じる
	fclose(fp);
}

void DataManager::Load()
{
	// ファイルを開く
	auto fileName = kDataFileName;
	int handle = FileRead_open(fileName);

	// 受け取り用のヘッダを用意
	SaveDataHeader header;

	// ヘッダを読み込み
	FileRead_read(&header, sizeof(header), handle);
	// セーブデータ本体を読み込み
	FileRead_read(&_saveData, sizeof(_saveData), handle);

	// ファイルを閉じる
	FileRead_close(handle);
}
