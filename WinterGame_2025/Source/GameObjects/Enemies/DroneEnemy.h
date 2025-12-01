#pragma once
#include "Enemy.h"
class DroneEnemy :
    public Enemy
{
public:
    DroneEnemy(Vector2 firstPos,std::shared_ptr<Player> pPlayer,int handle);
    ~DroneEnemy();

    void Init() override;
	void Update(Map& map) override;
    void Draw(Vector2 offset) override;
private:
    int _handle = -1;
};