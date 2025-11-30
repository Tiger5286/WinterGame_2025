#include "Stage.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	// 配置に必要な情報
	struct DataSetting
	{
		uint8_t chipW;	// チップ幅(いらない)
		uint8_t chipH;	// チップ高さ(いらない)
		uint8_t layerCount;	// レイヤー数
		uint8_t bitCount;	// 1チップに何ビット使用しているのか
	};
	// マップデータのヘッダ
	struct DataHeader
	{
		char identifier[4];	// 正しければ'FMF_'になっている
		uint32_t size;	// データのサイズ
		uint32_t width;	// データの幅
		uint32_t height;	// データの高さ
		DataSetting setting;	// データ配置に必要な情報
	};
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
	DataHeader header;
	FileRead_read(&header, sizeof(header), handle);

	// マップサイズを計算
	auto mapSize = header.width * header.height;

	// マップデータをロード
	_mapData.resize(mapSize);
	FileRead_read(_mapData.data(), sizeof(uint16_t) * mapSize, handle);

	// オブジェクトデータをロード
	_objectData.resize(mapSize);
	FileRead_read(_objectData.data(), sizeof(uint16_t) * mapSize, handle);

	// ファイルを閉じる
	FileRead_close(handle);
}
