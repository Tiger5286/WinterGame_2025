#include "Map.h"
#include "DxLib.h"
#include <fstream>
#include <sstream>

#include "BoxCollider.h"
#include "CircleCollider.h"

namespace
{
	// マップチップのサイズと描画倍率
	constexpr int CHIP_SIZE = 16;
	constexpr float DRAW_SCALE = 3.0f;

	// 画像のマップチップ数
	constexpr int GRAPH_CHIP_NUM_X = 528 / 16;
	constexpr int GRAPH_CHIP_NUM_Y = 624 / 16;
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
	constexpr int drawChipSize = CHIP_SIZE * DRAW_SCALE;
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

			int srcX = CHIP_SIZE * (chipNo % GRAPH_CHIP_NUM_X);
			int srcY = CHIP_SIZE * (chipNo / GRAPH_CHIP_NUM_X);

			int scrollPosX = posX - offset.x;
			int scrollPosY = posY - offset.y;

			// マップチップの描画
			DrawRectRotaGraph(
				scrollPosX, scrollPosY,
				srcX, srcY,
				CHIP_SIZE, CHIP_SIZE,
				DRAW_SCALE, 0.0f,
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

bool Map::IsCollision(std::shared_ptr<Collider> pCollider, Vector2& hitChipPos)
{
	for (int x = 0; x < _chipNumX; x++)
	{
		for (int y = 0; y < _chipNumY; y++)
		{
			// 0番のチップには当たり判定がないので無視
			if (_chipData[x][y] == 0) continue;

			// マップチップの位置とサイズを計算
			Vector2 chipPos = { x * CHIP_SIZE * DRAW_SCALE + (CHIP_SIZE * DRAW_SCALE / 2),y * CHIP_SIZE * DRAW_SCALE + (CHIP_SIZE * DRAW_SCALE / 2) };
			Vector2 chipSize = { CHIP_SIZE * DRAW_SCALE,CHIP_SIZE * DRAW_SCALE };

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
	return _chipNumX * CHIP_SIZE * DRAW_SCALE;
}

Vector2 Map::GetStageSize()
{
	return Vector2(_chipNumX * CHIP_SIZE * DRAW_SCALE, _chipNumY * CHIP_SIZE * DRAW_SCALE);
}

void Map::LoadMapData(std::string fileName)
{
	std::ifstream file(fileName);
	std::string line;
	std::vector<std::vector<int>> tempData;

	while (std::getline(file, line))
	{
		std::istringstream stream(line);	
		std::string field;	
		std::vector<int> row;

		while (getline(stream, field, ','))	
		{
			row.push_back(std::stoi(field));
		}
		tempData.push_back(row);
	}

	// Yが1多くて範囲外アクセスするので-1してます
	// これはcsvの最後に空行があるためです
	_chipNumY = static_cast<int>(tempData.size() - 1);
	if (_chipNumY > 0)
	{
		_chipNumX = static_cast<int>(tempData[0].size());
	}
	else
	{
		_chipNumX = 0;
	}
	_chipData = std::vector<std::vector<int>>(_chipNumX, std::vector<int>(_chipNumY, 0));
	for (int y = 0; y < _chipNumY; y++)
	{
		for (int x = 0; x < _chipNumX; x++)
		{
			_chipData[x][y] = tempData[y][x];
		}
	}
}
