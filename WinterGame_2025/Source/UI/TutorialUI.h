#pragma once
#include "../Utility/Geometry.h"

class Input;
class Player;

class TutorialUI
{
public:
	TutorialUI(Player& player);

	void Update(Input& input);
	void Draw(const Vector2& offset);

private:
	Player& _player;
};

