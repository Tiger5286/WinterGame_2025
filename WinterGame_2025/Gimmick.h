#pragma once
#include "GameObject.h"

class Player;

class Gimmick : public GameObject
{
public:
	Gimmick(std::shared_ptr<Player> pPlayer);
	virtual ~Gimmick();

	virtual void Init() override = 0;
	virtual void Update(Map& map) override = 0;
	virtual void Draw(Vector2 cameraOffset) override = 0;
private:
	std::shared_ptr<Player> _pPlayer;
};