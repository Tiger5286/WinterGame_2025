#pragma once
#include "GameObject.h"

enum class GimmickType
{
	Laser,
	Spike,
};

class Player;

class Gimmick : public GameObject
{
public:
	Gimmick(std::shared_ptr<Player> pPlayer);
	virtual ~Gimmick();

	virtual void Init() override = 0;
	virtual void Update(Map& map) override = 0;
	virtual void Update() = 0;
	virtual void Draw(Vector2 cameraOffset) override = 0;

	virtual GimmickType GetType() const = 0;
protected:
	std::shared_ptr<Player> _pPlayer;
};