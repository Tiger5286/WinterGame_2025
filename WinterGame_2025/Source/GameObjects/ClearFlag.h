#pragma once
#include "Gimmick.h"

class Player;

class ClearFlag : public GameObject
{
public:
	ClearFlag(Vector2 chipPos,std::shared_ptr<Player> pPlayer, int handle);
	~ClearFlag();

	virtual void Init() override;
	virtual void Update(Map& map) override;
	virtual void Draw(Vector2 cameraOffset) override;
private:
	int _handle;
	std::shared_ptr<Player> _pPlayer;
};

