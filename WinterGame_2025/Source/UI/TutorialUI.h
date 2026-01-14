#pragma once
#include "../Utility/Geometry.h"
#include <vector>

enum class TutorialUISteps
{
	None,
	Move,
	Shot,
	Jump,
	ChargeShot,
	Dash,
	WallJump,

	Num
};

class Input;
class Player;

class TutorialUI
{
public:
	TutorialUI(Player& player);
	~TutorialUI();

	void Update(Input& input);
	void Draw(const Vector2& offset);

	void DrawChargeGauge(const Vector2& drawPos);
	void DrawDashGauge(const Vector2& drawPos);

	void DrawMove(const Vector2& drawPos);
	void DrawShot(const Vector2& drawPos);
	void DrawJump(const Vector2& drawPos);
	void DrawChargeShot(const Vector2& drawPos);
	void DrawDash(const Vector2& drawPos);
	void DrawWallJump(const Vector2& drawPos);

private:
	Player& _player;

	using DrawFunc_t = void(TutorialUI::*)(const Vector2& drawPos);
	DrawFunc_t _draw = &TutorialUI::DrawMove;

	std::vector<int> _graphHandles;

	int _frame = 0;

	TutorialUISteps _step = TutorialUISteps::Move;
};