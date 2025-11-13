#pragma once
#include "Gimmick.h"

class Player;

class Laser : public Gimmick
{
public:
	//Laser(int handle,int laserLength,std::shared_ptr<Player> pPlayer);
	//Laser(Vector2 firstPos,int handle, int laserLength, std::shared_ptr<Player> pPlayer);
	Laser(Vector2 chipPos, std::shared_ptr<Player> pPlayer, int handle, int laserLength);
	~Laser();

	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;
private:
	int _handle;
	//std::shared_ptr<Player> _pPlayer;
	int _laserLength;

	Animation _launcherAnim;
	Animation _laserAnim;
	Animation _impactAnim;
};