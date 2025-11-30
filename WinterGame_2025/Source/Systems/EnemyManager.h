#pragma once
#include <memory>
#include <vector>
#include "../Utility/Geometry.h"

class Player;
class Map;
class Camera;
class Enemy;

/// <summary>
/// 敵の出現、削除などを管理するクラス
/// </summary>
class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<Map> pMap, std::shared_ptr<Camera> pCamera);
	~EnemyManager();

	void Update();
	void Draw();

	/// <summary>
	/// 設定された位置に敵を出現させる
	/// </summary>
	/// <param name="objectData">オブジェクトデータの配列</param>
	/// <param name="size">マップサイズ</param>
	void LoadEnemies(const std::vector<uint16_t>& objectData, Size size);

	const std::vector<std::shared_ptr<Enemy>>& GetEnemies() const { return _pEnemies; }

private:
	int _walkEnemyH = -1;
	int _flyEnemyH = -1;
	int _jumpEnemyH = -1;

	std::shared_ptr<Player> _pPlayer;
	std::shared_ptr<Map> _pMap;
	std::shared_ptr<Camera> _pCamera;
	std::vector<std::shared_ptr<Enemy>> _pEnemies;
};