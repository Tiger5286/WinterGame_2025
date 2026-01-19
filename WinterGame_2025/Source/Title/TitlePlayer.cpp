#include "TitlePlayer.h"
#include "Dxlib.h"
#include <cassert>
#include "../Game.h"
#include "../Systems/Input.h"
#include "TitleBullet.h"
#include "../Systems/SoundManager.h"

namespace
{
	const Vector2 kGraphSize = { 40.0f,40.0f };
	const Vector2 kFlashGraphSize = { 16.0f,16.0f };

	constexpr int kIdleAnimIndex = 0;
	constexpr int kFlashAnimIndex = 0;
	constexpr int kIdleAnimNum = 5;
	constexpr int kFlashAnimNum = 3;
	constexpr int kOneAnimFrame = 6;
	constexpr float kDrawScale = 3.0f;

	const Vector2 kPos = { 300,828 };
	const Vector2 kShotPos = { 360,850 };
}

TitlePlayer::TitlePlayer(std::array<std::shared_ptr<TitleBullet>, 10>& bullets):
	_bullets(bullets)
{
	// ‰æ‘œ‚ðƒ[ƒh
	_playerHandle = LoadGraph("data/Player/Player.png");
	assert(_playerHandle != -1);
	_shotHandle = LoadGraph("data/Player/Shot.png");
	assert(_shotHandle != -1);

	_anim.Init(_playerHandle, kIdleAnimIndex, kGraphSize, kIdleAnimNum, kOneAnimFrame, kDrawScale);
	_flashAnim.Init(_shotHandle, kFlashAnimIndex, kFlashGraphSize, kFlashAnimNum, kOneAnimFrame,kDrawScale,false);
	_flashAnim.SetEnd();
}

TitlePlayer::~TitlePlayer()
{
	DeleteGraph(_playerHandle);
	DeleteGraph(_shotHandle);
}

void TitlePlayer::Update(Input& input)
{
	if (input.IsTriggered("shot"))
	{
		for (auto& bullet : _bullets)
		{
			if (!bullet->GetIsAlive())
			{
				bullet->Shot();
				_flashAnim.SetFirst();
				SoundManager::GetInstance().PlaySoundGame("Shot");
				break;
			}
		}
	}
	
	_anim.Update();
	_flashAnim.Update();
}

void TitlePlayer::Draw()
{
	_anim.Draw(kPos, false);
	_flashAnim.Draw(kShotPos, false);
}