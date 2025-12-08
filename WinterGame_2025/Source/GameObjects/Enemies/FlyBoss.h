#pragma once
#include "Enemy.h"

class Camera;
class EnemyManager;

enum class FlyBossState
{
    Idle,
    AimAndTackle,
    SummonEnemies,
    Back,
};

class FlyBoss :
    public Enemy
{
public:
    FlyBoss(Vector2 pos, std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, EnemyManager& enemyManager, std::shared_ptr<EffectManager> pEffectManager, SceneManager& sceneManager, int handle);
    ~FlyBoss();

    void Init() override;
    void Update(Map& map) override;
    void UpdateAnytime() override;
    void Draw(Vector2 offset) override;

private:
    void Update();

	/// <summary>
	/// プレイヤーが左右どちらにいるかチェックし、_isPlayerOnLeftを更新する
	/// </summary>
	void CheckIsPlayerOnLeft();

private:
    int _handle = -1;

    std::shared_ptr<Camera> _pCamera;
    EnemyManager& _enemyManager;

    int _frame = 0;
    FlyBossState _state = FlyBossState::Idle;
	bool _isPlayerOnLeft = true;    // プレイヤーが左にいるかどうか
    bool _isOnLeft = false;     // 自身が左側にいるかどうか

    float _sinAngle = 0.0f;
};

