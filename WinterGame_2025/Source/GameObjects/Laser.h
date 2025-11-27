#pragma once
#include "Gimmick.h"

class Player;

class Laser : public Gimmick
{
public:
	Laser(Vector2 chipPos, std::shared_ptr<Player> pPlayer, int handle, int laserLength,bool isDownward = true);
	~Laser();

	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;
private:
	int _handle;
	int _laserLength;
	bool _isDownward;

	Animation _launcherAnim;
	Animation _laserAnim;
	Animation _impactAnim;
};