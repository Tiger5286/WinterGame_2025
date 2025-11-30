#pragma once
#include <memory>
#include <vector>
#include "../Utility/Geometry.h"

class Player;
class Gimmick;

/// <summary>
/// ギミックを生成、管理するクラス
/// </summary>
class GimmickManager
{
public:
	GimmickManager(std::shared_ptr<Player> pPlayer);
	~GimmickManager();
	void Update();
	void Draw(Vector2 offset);

	/// <summary>
	/// ギミックを生成する
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <param name="size">マップのサイズ</param>
	void LoadGimmicks(const std::vector<uint16_t>& objectData, Size size);

private:
	int _laserH = -1;

	std::shared_ptr<Player> _pPlayer;
	std::vector<std::shared_ptr<Gimmick>> _pGimmicks;
};

