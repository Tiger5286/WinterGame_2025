#pragma once
#include "GameObject.h"

class Player;

class Laser : public GameObject
{
public:
	Laser(Vector2 firstPos,int handle,std::shared_ptr<Player> pPlayer);
	~Laser();

	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;
private:
	int _handle;
	std::shared_ptr<Player> _pPlayer;

	Animation _launcherAnim;
	Animation _laserAnim;
	Animation _impactAnim;
};

