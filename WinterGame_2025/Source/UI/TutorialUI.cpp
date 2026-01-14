#include "TutorialUI.h"
#include "Dxlib.h"
#include "../GameObjects/Player.h"

TutorialUI::TutorialUI(Player& player):
	_player(player)
{
}

void TutorialUI::Update(Input& input)
{

}

void TutorialUI::Draw(const Vector2& offset)
{
	Vector2 drawPos;
	drawPos.x = _player._pos.x - offset.x;
	drawPos.y = _player._pos.y - 100 - offset.y;
	DrawString(drawPos.x, drawPos.y, "Use Arrow Keys to Move", GetColor(255, 255, 255));
}