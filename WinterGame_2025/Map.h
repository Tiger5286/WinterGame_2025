#pragma once
#include <vector>
#include "Geometry.h"
#include <memory>

class Collider;

class Map
{
public:
	Map(int handle);
	~Map();

	void Draw(Vector2 offset);

	bool IsCollision(std::shared_ptr<Collider> pCollider, Vector2& hitChipPos);

	int GetStageWidth() const;

private:
	void LoadMapData();
private:
	int _handle;

	// マップデータ
	static constexpr int CHIP_NUM_X = 120;
	static constexpr int CHIP_NUM_Y = 23;
	std::vector<std::vector<int>> _chipData;
};

