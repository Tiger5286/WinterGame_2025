#include "FlyBoss.h"
#include "Dxlib.h"
#include "../../Utility/CircleCollider.h"

namespace
{
	constexpr int kGraphSize = 48;
	constexpr float kDrawScale = 6.0f;

	constexpr int kAnimNum = 5;
	constexpr int kOneAnimFrame = 6;

	constexpr int kColliderRadius = 70;

	constexpr int kHp = 100;
	constexpr int kScore = 10000;
}

FlyBoss::FlyBoss(Vector2 pos, std::shared_ptr<Player> pPlayer, std::shared_ptr<EffectManager> pEffectManager, SceneManager& sceneManager,int handle):
	Enemy(kHp, kScore,pPlayer,pEffectManager,sceneManager),
	_handle(handle)
{
	_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, kOneAnimFrame, kDrawScale);
	_pos = ChipPosToGamePos(pos);
	_pCollider = std::make_shared<CircleCollider>(_pos, kColliderRadius);
}

FlyBoss::~FlyBoss()
{
}

void FlyBoss::Init()
{
}

void FlyBoss::Update(Map& map)
{
	Update();
}

void FlyBoss::Update()
{
	_nowAnim.Update();
}

void FlyBoss::Draw(Vector2 offset)
{
	_nowAnim.Draw(_pos - offset, false);
#ifdef _DEBUG
	_pCollider->Draw(offset);
#endif
}
