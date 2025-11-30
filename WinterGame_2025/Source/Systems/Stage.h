#pragma once
#include <string>
#include <vector>

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

class Stage
{
	Stage();
	~Stage();

	void LoadData(const std::string& filePath);

private:
	std::vector<uint16_t> _mapData;	// マップデータ
	std::vector<uint16_t> _objectData;	// オブジェクトデータ
};

