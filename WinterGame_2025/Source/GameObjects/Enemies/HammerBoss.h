#pragma once
#include "Enemy.h"

class Camera;
class BulletManager;

enum class HammerBossState
{
    Idle,
    WaveAttack,
    FallBallAttack,
    Death,
};

class HammerBoss :
    public Enemy
{
public:
    HammerBoss(Vector2 firstPos, std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, BulletManager& bulletManager, std::shared_ptr<Camera> pCamera, SceneManager& sceneManager, int handle);
    ~HammerBoss();

    void Init() override;
    void Update(Map& map) override;
    void Draw(Vector2 offset) override;

	void TakeDamage(int damage) override;
    bool GetIsAlive() const override;

private:
    int _handle;

    int _frame = 0;
    HammerBossState _state = HammerBossState::Idle;

    // ボール落下攻撃のボールが落ちてくるエリアの番号
    int _fBArea = 0;

	std::shared_ptr<Collider> _pAttackCollider;

    std::shared_ptr<Camera> _pCamera;
	BulletManager& _bulletManager;

    Animation _idleAnim;
    Animation _attackAnim;
	Animation _rapidAttackAnim;
};

