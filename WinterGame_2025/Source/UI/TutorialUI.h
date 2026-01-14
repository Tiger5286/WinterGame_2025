#pragma once
#include "../Utility/Geometry.h"
#include <vector>

class Input;
class Player;

class TutorialUI
{
public:
	TutorialUI(Player& player);
	~TutorialUI();

	void Update(Input& input);
	void Draw(const Vector2& offset);

private:
	Player& _player;

	std::vector<int> _graphHandles;
};

