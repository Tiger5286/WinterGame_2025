#pragma once
#include "Enemy.h"

class Camera;
class EnemyManager;
class BulletManager;

enum class FlyBossState
{
    Idle,
    AimAndTackle,   // 狙って突進攻撃
    FallBall,       // 球を落とす攻撃
    SummonEnemies,  // 雑魚召喚
    Back,   // 規定の位置に戻る
    Death,  // 死亡
};

class FlyBoss :
    public Enemy
{
public:
    FlyBoss(Vector2 pos, std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, EnemyManager& enemyManager,BulletManager& bulletManager, std::shared_ptr<EffectManager> pEffectManager, SceneManager& sceneManager, int handle, int barrierH);
    ~FlyBoss();

    void Init() override;
    void Update(Map& map) override;
    void UpdateAnytime() override;
    void Draw(Vector2 offset) override;

    void TakeDamage(int damage,Bullet& bullet) override;
    bool GetIsAlive() const override;

private:
    void Update();

	/// <summary>
	/// プレイヤーが左右どちらにいるかチェックし、_isPlayerOnLeftを更新する
	/// </summary>
	void CheckIsPlayerOnLeft();

	/// <summary>
	/// 自身が左右どちらにいるかチェックし、_isOnLeftを更新する
	/// </summary>
	void CheckIsOnLeft();

private:
    int _handle;
	int _barrierH;

    std::shared_ptr<Camera> _pCamera;
    EnemyManager& _enemyManager;
	BulletManager& _bulletManager;

    int _frame = 0;
	int _drawBarrierFrame = 0;
    FlyBossState _state = FlyBossState::Idle;
	FlyBossState _prevState = FlyBossState::Idle;
	bool _isPlayerOnLeft = true;    // プレイヤーが左にいるかどうか
    bool _isOnLeft = false;     // 自身が左側にいるかどうか

    float _sinAngle = 0.0f;

    Animation _barrierAnim;
};

