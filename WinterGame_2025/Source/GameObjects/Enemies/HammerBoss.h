#pragma once
#include "Enemy.h"

class Camera;

class HammerBoss :
    public Enemy
{
public:
    HammerBoss(Vector2 firstPos, std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, std::shared_ptr<Camera> pCamera, SceneManager& sceneManager, int handle);
    ~HammerBoss();

    void Init() override;
    void Update(Map& map) override;
    void Draw(Vector2 offset) override;

private:
    int _handle;

    std::shared_ptr<Camera> _pCamera;

    Animation _idleAnim;
    Animation _attackAnim;
};

