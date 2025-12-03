#include "SceneClear.h"
#include "../Systems/Input.h"
#include "Dxlib.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"
#include "DebugScene.h"
#include "../Game.h"
#include <cassert>

SceneClear::SceneClear(SceneManager& manager,int score):
	SceneBase(manager),
	_score(score)
{
	_bgHandle = LoadGraph("data/Map/Bg.png");
	assert(_bgHandle != -1);
	_clearLogoHandle = LoadGraph("data/UI/StageClear.png");
	assert(_clearLogoHandle != -1);
}

SceneClear::~SceneClear()
{
	DeleteGraph(_bgHandle);
	DeleteGraph(_clearLogoHandle);
}

void SceneClear::Init()
{
}

void SceneClear::Update(Input& input)
{
	if (input.IsTriggered("decision"))
	{
		_manager.ChangeSceneWithFade(std::make_shared<SceneStageSelect>(_manager));
	}

#ifdef _DEBUG
	if (input.IsTriggered("select"))
	{
		_manager.ChangeScene(std::make_shared<DebugScene>(_manager));
	}
#endif
}

void SceneClear::Draw()
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	DrawExtendGraph(0, 0, screenW, screenH, _bgHandle, false);

	DrawRotaGraph(screenW / 2, screenH / 2 - 100, 0.75, 0.0, _clearLogoHandle, true);

	_dispScore = std::lerp(_dispScore, static_cast<float>(_score), 0.02f);

	if (_score != 0)
	{
		DrawFormatString(screenW / 2, screenH / 2 + 130, 0xffffff, "Score:%d", static_cast<int>(_dispScore) + 1);	// なぜか_score-1で止まるので+1して補正
	}
	else
	{
		DrawFormatString(screenW / 2, screenH / 2 + 130, 0xffffff, "Score:%d", static_cast<int>(_dispScore));	// スコアが0のときに+1するとダメなのでそのまま表示
	}
	
	DrawString(screenW / 2, screenH / 2 + 250, "Press A to Back to Stage Select", 0xffffff);

#ifdef _DEBUG
	DrawString(0, 0, "SceneClear", 0xffffff);
#endif
}
