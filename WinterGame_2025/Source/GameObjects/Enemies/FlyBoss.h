#pragma once
#include "Enemy.h"
class FlyBoss :
    public Enemy
{
public:
    FlyBoss(Vector2 pos, std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, SceneManager& sceneManager,int handle);
    ~FlyBoss();

    void Init() override;
    void Update(Map& map) override;
    void Draw(Vector2 offset) override;

private:
    void Update();

private:
    int _handle = -1;
};

