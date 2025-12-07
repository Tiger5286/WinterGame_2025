#pragma once
#include "Enemy.h"

enum class FlyBossState
{
    Idle,
    AimTackle,
};

class FlyBoss :
    public Enemy
{
public:
    FlyBoss(Vector2 pos, std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, SceneManager& sceneManager,int handle);
    ~FlyBoss();

    void Init() override;
    void Update(Map& map) override;
    void UpdateAnytime() override;
    void Draw(Vector2 offset) override;

private:
    void Update();

private:
    int _handle = -1;

    int _frame = 0;
    FlyBossState _state = FlyBossState::Idle;
    bool _isPlayerOnLeft = true;

    float _sinAngle = 0.0f;
};

