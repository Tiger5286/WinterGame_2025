#include "Map.h"
#include "DxLib.h"
#include <fstream>
#include <sstream>

#include "BoxCollider.h"
#include "CircleCollider.h"

namespace
{
	// 画像のマップチップ数
	constexpr int GRAPH_CHIP_NUM_X = 528 / 16;
	constexpr int GRAPH_CHIP_NUM_Y = 624 / 16;

	// マップチップのサイズと描画倍率
	constexpr int CHIP_SIZE = 16;
	constexpr float DRAW_SCALE = 3.0f;
}

Map::Map(int handle) :
	_handle(handle)
{
	_chipData.resize(CHIP_NUM_X, std::vector<int>(CHIP_NUM_Y, 0));
	LoadMapData();
}

Map::~Map()
{
}

void Map::Draw(Vector2 offset)
{
	constexpr int drawChipSize = CHIP_SIZE * DRAW_SCALE;
	constexpr int drawChipSizeHalf = drawChipSize / 2;

	// マップチップの描画
	for (int y = 0; y < CHIP_NUM_Y; y++)
	{
		for (int x = 0; x < CHIP_NUM_X; x++)
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

			Vector2 chipPos = { x * CHIP_SIZE * DRAW_SCALE + (CHIP_SIZE * DRAW_SCALE / 2),y * CHIP_SIZE * DRAW_SCALE + (CHIP_SIZE * DRAW_SCALE / 2) };
			// マップチップの枠表示
			//DrawBox(chipPos.x - drawChipSizeHalf - offset.x,
			//	chipPos.y - drawChipSizeHalf - offset.y,
			//	chipPos.x + drawChipSizeHalf - offset.x,
			//	chipPos.y + drawChipSizeHalf - offset.y,
			//	0x00ff00, false);

			// マップチップの中心点表示
			//DrawPixel(chipPos.x - offset.x, chipPos.y - offset.y, 0xffffff);
		}
	}
}

bool Map::IsCollision(std::shared_ptr<Collider> pCollider, Vector2& hitChipPos)
{
	for (int x = 0; x < CHIP_NUM_X; x++)
	{
		for (int y = 0; y < CHIP_NUM_Y; y++)
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
	return CHIP_NUM_X * CHIP_SIZE * DRAW_SCALE;
}

void Map::LoadMapData()
{
	// std::ifstreamとは ファイルからデータを読み込むためのクラス
	std::ifstream file("data/Map/Map.csv");
	std::string line;

	// getline関数で1行ずつ読み込む
	int y = 0;
	// std::getlineとは ファイルや文字列から1行ずつデータを読み込むための関数
	// std::getlineの引数には、読み込み先のストリーム、読み込んだ行を格納する文字列変数が必要
	while (std::getline(file, line) && y < CHIP_NUM_Y)
	{
		// std::istringstreamとは 文字列をストリームとして扱うためのクラス
		std::istringstream stream(line);	// 読み込んだ行をストリームに変換
		std::string field;	// 各フィールドを格納するための文字列変数
		// フィールドとは データベースやCSVファイルなどで、1つのレコード内の個々のデータ要素を指す

		// カンマ区切りで分割して読み込む
		int x = 0;
		// ここでは getline関数を使って、カンマ（,）を区切り文字として各フィールドを読み込む
		while (getline(stream, field, ',') && x < CHIP_NUM_X)	// std::getlineの第3引数には区切り文字を指定できる
		{
			_chipData[x][y] = std::stoi(field); // 文字列を整数に変換して格納
			x++;
		}
		y++;
	}
}
