#include "SceneGameOver.h"
#include "Dxlib.h"
#include "../Systems/Input.h"
#include "../Systems/SoundManager.h"
#include "../Game.h"

#include "SceneManager.h"
#include "DebugScene.h"
#include "SceneStageSelect.h"

#include <cassert>

SceneGameOver::SceneGameOver(SceneManager& manager,Stages playedStage,int score):
	SceneBase(manager),
	_isSelectRestart(true),
	_score(score),
	_playedStage(playedStage)
{
	_bgHandle = LoadGraph("data/Map/Bg.png");
	assert(_bgHandle != -1);
	_gameOverHandle = LoadGraph("data/UI/GameOver.png");
	assert(_gameOverHandle != -1);
	_restartTextHandle = LoadGraph("data/UI/RestartText.png");
	assert(_restartTextHandle != -1);
	_backTextHandle = LoadGraph("data/UI/BackToStageSelectText.png");
	assert(_backTextHandle != -1);

	SoundManager::GetInstance().LoadSound("GameOverBgm", "data/Sounds/BGM/GameOverBGM.ogg", SoundType::BGM);
	SoundManager::GetInstance().PlaySoundGame("GameOverBgm", true, true);
}

SceneGameOver::~SceneGameOver()
{
	DeleteGraph(_bgHandle);
	DeleteGraph(_gameOverHandle);
	DeleteGraph(_restartTextHandle);
	DeleteGraph(_backTextHandle);

	SoundManager::GetInstance().DeleteSound("GameOverBgm");
}

void SceneGameOver::Init()
{
}

void SceneGameOver::Update(Input& input)
{
	if (input.IsTriggered("up") || input.IsTriggered("down"))
	{
		_isSelectRestart = !_isSelectRestart;
	}
	if (input.IsTriggered("decision"))
	{
		SoundManager::GetInstance().StopSound("GameOverBgm", true);
		if (_isSelectRestart)
		{
			_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, _playedStage,_score),FadeState::CircleFadeIn);
		}
		else
		{
			auto selectableStage = StageToSelectableStage(_playedStage);
			_manager.ChangeSceneWithFade(std::make_shared<SceneStageSelect>(_manager, _playedStage));
		}
	}
}

void SceneGameOver::Draw()
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	DrawExtendGraph(0, 0, screenW, screenH, _bgHandle, false);
	DrawRotaGraph(screenW / 2, screenH / 2, 0.7, 0.0, _gameOverHandle, true);

	if (_isSelectRestart)
	{
		DrawRotaGraph(screenW / 2, screenH / 2 + 300, 0.6, 0.0, _restartTextHandle, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawRotaGraph(screenW / 2, screenH / 2 + 400, 0.4, 0.0, _backTextHandle, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawRotaGraph(screenW / 2, screenH / 2 + 300, 0.4, 0.0, _restartTextHandle, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawRotaGraph(screenW / 2, screenH / 2 + 400, 0.6, 0.0, _backTextHandle, true);
	}
}