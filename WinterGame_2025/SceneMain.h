#pragma once
#include "SceneBase.h"
#include <memory>

class Player;

class SceneMain : public SceneBase
{
public:
	SceneMain();
	~SceneMain();

	void Update() override;
	void Draw() override;
private:
	int _frameCount;
	std::shared_ptr<Player> _pPlayer;
};

