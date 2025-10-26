#pragma once
#include "SceneBase.h"
#include <memory>

class Input;
class Player;

class SceneMain : public SceneBase
{
public:
	SceneMain();
	~SceneMain();

	void Init() override;
	void Update(Input input) override;
	void Draw() override;
private:
	int _frameCount;
	std::shared_ptr<Player> _pPlayer;
};

