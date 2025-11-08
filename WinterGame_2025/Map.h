#pragma once
#include <vector>
#include "Geometry.h"
#include <memory>
#include <string>

class Collider;

class Map
{
public:
	Map(int handle);
	~Map();

	void Draw(Vector2 offset);

	bool IsCollision(std::shared_ptr<Collider> pCollider, Vector2& hitChipPos);

	int GetStageWidth() const;

	void LoadMapData(std::string fileName);

private:
	int _handle;

	// マップデータ
	int _chipNumX;
	int _chipNumY;
	std::vector<std::vector<int>> _chipData;
};