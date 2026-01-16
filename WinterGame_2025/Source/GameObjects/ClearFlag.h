#pragma once
#include "Gimmick.h"
#include <vector>
#include "../Systems/Animation.h"

class Player;

class ClearFlag : public GameObject
{
public:
	ClearFlag(Vector2 chipPos,std::shared_ptr<Player> pPlayer, int handle);
	~ClearFlag();

	virtual void Init() override;
	virtual void Update(Map& map) override;
	void Update();
	virtual void Draw(Vector2 cameraOffset) override;

	void InitPosFromStage(std::vector<uint16_t>& objectData, Size mapSize);
private:
	int _handle;
	std::shared_ptr<Player> _pPlayer;
	Animation _anim;
};

