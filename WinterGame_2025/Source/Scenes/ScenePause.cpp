#include "ScenePause.h"
#include "SceneManager.h"
#include "../Systems/Input.h"
#include "Dxlib.h"
#include "../Game.h"

ScenePause::ScenePause(SceneManager& manager) :
	SceneBase(manager)
{
}

ScenePause::~ScenePause()
{
}

void ScenePause::Init()
{
}

void ScenePause::Update(Input& input)
{
	if (input.IsTriggered("start"))
	{
		_manager.PopScene();
		return;
	}
}

void ScenePause::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);	// 半透明にする
	DrawBox(0, 0, GlobalConstants::kScreenWidth, GlobalConstants::kScreenHeight, 0x000000, true);	// 黒い四角を描画
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドモードを元に戻す

	DrawString(GlobalConstants::kScreenWidth / 2, GlobalConstants::kScreenHeight / 2, "pause scene", 0xffffff);
}