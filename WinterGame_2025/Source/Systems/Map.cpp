#include "Map.h"
#include "DxLib.h"
#include <fstream>
#include <sstream>

#include "../Utility/BoxCollider.h"
#include "../Utility/CircleCollider.h"

namespace
{
	// マップチップのサイズと描画倍率
	constexpr int kChipSize = 16;
	constexpr float kDrawScale = 3.0f;

	// 画像のマップチップ数
	constexpr int kGraphChipNumX = 528 / kChipSize;
	constexpr int kGraphChipNumY = 624 / kChipSize;
}

Map::Map(int handle) :
	_handle(handle),
	_chipNumX(0),
	_chipNumY(0)
{
}

Map::~Map()
{
}

void Map::Draw(Vector2 offset)
{
	constexpr int drawChipSize = kChipSize * kDrawScale;
	constexpr int drawChipSizeHalf = drawChipSize / 2;

	// マップチップの描画
	for (int y = 0; y < _chipNumY; y++)
	{
		for (int x = 0; x < _chipNumX; x++)
		{

			int posX = x * drawChipSize + drawChipSizeHalf;
			int posY = y * drawChipSize + drawChipSizeHalf;

			// 設置するチップ
			int chipNo = _chipData[x][y];

			int srcX = kChipSize * (chipNo % kGraphChipNumX);
			int srcY = kChipSize * (chipNo / kGraphChipNumX);

			int scrollPosX = posX - offset.x;
			int scrollPosY = posY - offset.y;

			// マップチップの描画
			DrawRectRotaGraph(
				scrollPosX, scrollPosY,
				srcX, srcY,
				kChipSize, kChipSize,
				kDrawScale, 0.0f,
				_handle, true
			);

			//Vector2 chipPos = { x * CHIP_SIZE * DRAW_SCALE + (CHIP_SIZE * DRAW_SCALE / 2),y * CHIP_SIZE * DRAW_SCALE + (CHIP_SIZE * DRAW_SCALE / 2) };
			//// マップチップの枠表示
			//DrawBox(chipPos.x - drawChipSizeHalf - offset.x,
			//	chipPos.y - drawChipSizeHalf - offset.y,
			//	chipPos.x + drawChipSizeHalf - offset.x,
			//	chipPos.y + drawChipSizeHalf - offset.y,
			//	0x00ff00, false);

			//// マップチップの中心点表示
			//DrawPixel(chipPos.x - offset.x, chipPos.y - offset.y, 0xffffff);
		}
	}
}

void Map::Draw2(Vector2 offset)
{
	constexpr int drawChipSize = kChipSize * kDrawScale;
	constexpr int drawChipSizeHalf = drawChipSize / 2;

	// マップチップの描画
	for (int h = 0; h < _mapSize.h; h++)
	{
		for (int w = 0; w < _mapSize.w; w++)
		{
			// チップ番号を取得
			int chipNo = _mapData[w + h * _mapSize.w];
			// 0番のチップは描画しない
			if (chipNo == 0) continue;

			// 描画位置を計算
			int DrawPosX = w * drawChipSize + drawChipSizeHalf - offset.x;
			int DrawPosY = h * drawChipSize + drawChipSizeHalf - offset.y;

			// 描画するチップの画像内位置を計算
			int srcX = kChipSize * (chipNo % kGraphChipNumX);
			int srcY = kChipSize * (chipNo / kGraphChipNumX);

			// マップチップの描画
			DrawRectRotaGraph(
				DrawPosX, DrawPosY,
				srcX, srcY,
				kChipSize, kChipSize,
				kDrawScale, 0.0f,
				_handle, true
			);
		}
	}
}

bool Map::IsCollision(std::shared_ptr<Collider> pCollider, Vector2& hitChipPos)
{
	for (int x = 0; x < _chipNumX; x++)
	{
		for (int y = 0; y < _chipNumY; y++)
		{
			// 0番のチップには当たり判定がないので無視
			if (_chipData[x][y] == 0) continue;

			// マップチップの位置とサイズを計算
			Vector2 chipPos = { x * kChipSize * kDrawScale + (kChipSize * kDrawScale / 2),y * kChipSize * kDrawScale + (kChipSize * kDrawScale / 2) };
			Vector2 chipSize = { kChipSize * kDrawScale,kChipSize * kDrawScale };

			if (pCollider->GetType() == Collider::Type::Circle)
			{
				float hitDisX = pCollider->GetRadius() + chipSize.x / 2;
				float hitDisY = pCollider->GetRadius() + chipSize.y / 2;
				float disX = abs(pCollider->GetPos().x - chipPos.x);
				float disY = abs(pCollider->GetPos().y - chipPos.y);
				if (disX < hitDisX && disY < hitDisY)
				{
					hitChipPos = chipPos;
					return true;
				}
			}
			else if (pCollider->GetType() == Collider::Type::Box)
			{
				float hitDisX = pCollider->GetSize().x / 2 + chipSize.x / 2;
				float hitDisY = pCollider->GetSize().y / 2 + chipSize.y / 2;
				float disX = abs(pCollider->GetPos().x - chipPos.x);
				float disY = abs(pCollider->GetPos().y - chipPos.y);
				if (disX < hitDisX && disY < hitDisY)
				{
					hitChipPos = chipPos;
					return true;
				}
			}
		}
	}
	//DrawFormatString(100, 100, 0xffffff, "%.2f,%.2f", pCollider->GetPos().x, pCollider->GetPos().y);

	return false;
}

int Map::GetStageWidth() const
{
	return _chipNumX * kChipSize * kDrawScale;
}

Vector2 Map::GetStageSize()
{
	return Vector2(_chipNumX * kChipSize * kDrawScale, _chipNumY * kChipSize * kDrawScale);
}

void Map::LoadMapData(std::string fileName)
{
	std::ifstream file(fileName);	// ファイルを開く
	std::string line;	// 1行分のデータを格納する変数
	std::vector<std::vector<int>> tempData;	// 一時的にマップデータを格納する変数

	while (std::getline(file, line))	// 1行ずつ読み込む
	{
		std::istringstream stream(line);	// 文字列ストリームを作成
		std::string field;	// カンマで区切られた各フィールドを格納する変数
		std::vector<int> row;	// 1行分のデータを格納する変数

		while (getline(stream, field, ','))		// カンマで区切られたフィールドを読み込む
		{
			row.push_back(std::stoi(field));	// 文字列を整数に変換して行に追加
		}
		tempData.push_back(row);	// 行をマップデータに追加
	}

	// Yが1多くて範囲外アクセスするので-1してます
	// これはcsvの最後に空行があるためです
	_chipNumY = static_cast<int>(tempData.size() - 1);	// マップの行数を取得
	if (_chipNumY > 0)	// データが存在する場合
	{
		_chipNumX = static_cast<int>(tempData[0].size());	// マップの列数を取得
	}
	else
	{
		_chipNumX = 0;	// データが存在しない場合は0に設定
	}
	_chipData = std::vector<std::vector<int>>(_chipNumX, std::vector<int>(_chipNumY, 0));	// マップデータの初期化
	for (int y = 0; y < _chipNumY; y++)	// データを転置して格納
	{
		for (int x = 0; x < _chipNumX; x++)
		{
			_chipData[x][y] = tempData[y][x];
		}
	}
}

void Map::SetMapData(const std::vector<uint16_t>& mapData, const Size& mapSize)
{
	_mapSize = mapSize;
	_mapData = mapData;
}
