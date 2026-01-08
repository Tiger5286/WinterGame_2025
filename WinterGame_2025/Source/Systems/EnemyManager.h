#pragma once
#include <memory>
#include <vector>
#include "../Utility/Geometry.h"
#include "../Game.h"

class Player;
class Map;
class Camera;
class GimmickManager;
class EffectManager;
class ItemManager;
class BulletManager;
class SceneMain;
class SceneManager;

class Enemy;

/// <summary>
/// 敵の出現、削除などを管理するクラス
/// </summary>
class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> pPlayer,
		std::shared_ptr<Map> pMap,
		std::shared_ptr<Camera> pCamera,
		std::shared_ptr<GimmickManager> pGimmickManager,
		std::shared_ptr<EffectManager> pEffectManager,
		ItemManager& itemManager,
		BulletManager& bulletManager,
		SceneMain& sceneMain,
		SceneManager& sceneManager);
	~EnemyManager();

	void Update();
	void Draw();

	/// <summary>
	/// ステージデータをもとに敵を生成する
	/// </summary>
	/// <param name="objectData">オブジェクトデータの配列</param>
	/// <param name="size">マップサイズ</param>
	void LoadEnemies(const std::vector<uint16_t>& objectData, Size size);

	/// <summary>
	/// 敵を生成する
	/// </summary>
	/// <param name="enemyData">敵の種類</param>
	/// <param name="pos">生成する位置</param>
	void Create(ObjectData enemyData, Vector2 pos, bool isChipPos = true);

	/// <summary>
	/// 存在するボスの合計体力を取得する
	/// </summary>
	/// <returns>存在するボスの合計体力</returns>
	int GetTotalBossHp() const;

	const std::vector<std::shared_ptr<Enemy>>& GetEnemies() const { return _pEnemies; }

private:
	int _walkEnemyH = -1;
	int _flyEnemyH = -1;
	int _jumpEnemyH = -1;
	int _droneEnemyH = -1;
	int _barrierH = -1;

	bool _isAddEnemy = false;

	std::shared_ptr<Player> _pPlayer;
	std::shared_ptr<Map> _pMap;
	std::shared_ptr<Camera> _pCamera;
	std::shared_ptr<GimmickManager> _pGimmickManager;
	std::shared_ptr<EffectManager> _pEffectManager;
	std::vector<std::shared_ptr<Enemy>> _pEnemies;

	ItemManager& _itemManager;
	BulletManager& _bulletManager;

	SceneMain& _sceneMain;
	SceneManager& _sceneManager;
};