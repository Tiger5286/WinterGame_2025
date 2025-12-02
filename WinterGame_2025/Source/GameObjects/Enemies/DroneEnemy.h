#pragma once
#include "Enemy.h"

enum class DroneEnemyState
{
    Search,
    Attack,
    Return
};

class DroneEnemy :
    public Enemy
{
public:
    DroneEnemy(Vector2 firstPos,std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, SceneManager& sceneManager, int handle);
    ~DroneEnemy();

    void Init() override;
	void Update(Map& map) override;
    void Draw(Vector2 offset) override;
private:
    int _handle = -1;
    int _frame = 0;
    float _firstPosY;
    DroneEnemyState _state = DroneEnemyState::Search;
};