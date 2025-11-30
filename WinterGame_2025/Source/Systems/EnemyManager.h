#pragma once
#include <memory>
#include <vector>

class Enemy;

/// <summary>
/// 敵の出現、削除などを管理するクラス
/// </summary>
class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	void Init();
	void Update();
	void Draw();

	//void LoadEnemyData

private:
	std::vector<std::shared_ptr<Enemy>> _enemies;
};