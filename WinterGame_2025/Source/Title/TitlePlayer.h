#pragma once
#include "../Systems/Animation.h"
#include <array>
#include <memory>

class Input;
class TitleBullet;

class TitlePlayer
{
public:
	TitlePlayer(std::array<std::shared_ptr<TitleBullet>, 10>& bullets);
	~TitlePlayer();

	void Update(Input& input);
	void Draw();

private:
	int _playerHandle;
	int _shotHandle;

	std::array<std::shared_ptr<TitleBullet>, 10>& _bullets;

	Animation _anim;
	Animation _flashAnim;
};

