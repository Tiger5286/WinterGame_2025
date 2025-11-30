#pragma once
#include <memory>
#include <vector>
#include "../Utility/Geometry.h"

class Player;
class Enemy;

/// <summary>
/// 敵の出現、削除などを管理するクラス
/// </summary>
class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> pPlayer);
	~EnemyManager();

	void Update();
	void Draw();

	/// <summary>
	/// 設定された位置に敵を出現させる
	/// </summary>
	/// <param name="objectData">オブジェクトデータの配列</param>
	/// <param name="size">マップサイズ</param>
	void Spawn(const std::vector<uint16_t>& objectData, Size size);

private:
	int _walkEnemyH = -1;
	int _flyEnemyH = -1;
	int _jumpEnemyH = -1;

	std::shared_ptr<Player> _pPlayer;
	std::vector<std::shared_ptr<Enemy>> _enemies;
};