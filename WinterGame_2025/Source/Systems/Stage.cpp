#include "Stage.h"
#include "Dxlib.h"
#include <cassert>

namespace
{

}

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::LoadData(const std::string& filePath)
{
	// ファイルを開く
	auto handle = FileRead_open(filePath.c_str());
	assert(handle != -1);

	// ヘッダをロード
	FileRead_read(&_header, sizeof(_header), handle);

	// マップサイズを計算
	auto mapSize = _header.width * _header.height;

	// マップデータをロード
	_mapData.resize(mapSize);
	FileRead_read(_mapData.data(), sizeof(uint16_t) * mapSize, handle);

	// オブジェクトデータをロード
	_objectData.resize(mapSize);
	FileRead_read(_objectData.data(), sizeof(uint16_t) * mapSize, handle);

	// ファイルを閉じる
	FileRead_close(handle);
}
