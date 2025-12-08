#pragma once
#include "GameObject.h"

class Player;

class FallBall :
    public GameObject
{
public:
    FallBall(std::shared_ptr<Player> pPlayer,int handle);
    ~FallBall();

    void Init() override;
    void Update(Map& map) override;
    void Draw(Vector2 offset) override;

    void Shot(Vector2 pos);

    bool GetIsAlive() { return _isAlive; }

private:
    int _handle;
    std::shared_ptr<Player> _pPlayer;

    bool _isBounced = false;

    bool _isAlive = false;
};

