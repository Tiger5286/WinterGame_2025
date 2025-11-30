#pragma once
#include <vector>
#include "../Utility/Geometry.h"
#include <memory>
#include <string>

class Collider;

class Map
{
public:
	Map(int handle);
	~Map();

	void Draw(Vector2 offset);
	void Draw2(Vector2 offset);

	bool IsCollision(std::shared_ptr<Collider> pCollider, Vector2& hitChipPos);

	int GetStageWidth() const;
	Vector2 GetStageSize();

	void LoadMapData(std::string fileName);

	void SetMapData(const std::vector<uint16_t>& mapData, const Size& mapSize);

private:
	int _handle;

	// マップデータ
	int _chipNumX;
	int _chipNumY;
	std::vector<std::vector<int>> _chipData;

	// 新マップデータ
	Size _mapSize;
	std::vector<uint16_t> _mapData;
};