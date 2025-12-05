#include "SceneStageSelect.h"
#include "../Systems/Input.h"
#include "SceneManager.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "Dxlib.h"
#include "DebugScene.h"
#include "../Game.h"
#include <cassert>

namespace
{
	constexpr int kUIControllInterval = 10;
	constexpr int kUIMoveScale = 40;
	constexpr float kUIDrawScale = 0.5f;
	constexpr float kUIDrawScaleHalf = kUIDrawScale / 2;
}

SceneStageSelect::SceneStageSelect(SceneManager& manager, int selectIndex):
	SceneBase(manager),
	_selectIndex(selectIndex),
	_frame(kUIControllInterval)
{
	_bgHandle = LoadGraph("data/map/bg.png");
	assert(_bgHandle != -1);
	_stageUIHandle = LoadGraph("data/UI/StageUI.png");
	assert(_stageUIHandle != -1);

	_stageList =
	{
		"Tutorial Stage",
		"Stage 1",
		"Stage 2",
		"Boss Stage"
	};
	_execTable["Tutorial Stage"] = [this]() {
		_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager,Stages::Tutorial),FadeState::CircleFadeIn);
		};
	_execTable["Stage 1"] = [this]() {
		_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, Stages::Stage1), FadeState::CircleFadeIn);
		};
	_execTable["Stage 2"] = [this]() {
		_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, Stages::Stage2), FadeState::CircleFadeIn);
		};
	_execTable["Boss Stage"] = [this]() {
		_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, Stages::Boss), FadeState::CircleFadeIn);
		};

	if (_selectIndex == static_cast<int>(Stages::Num) - 2)
	{
		_isUIMoveRight = false;
		_frame = -kUIControllInterval;
	}
}

SceneStageSelect::~SceneStageSelect()
{
	DeleteGraph(_bgHandle);
	DeleteGraph(_stageUIHandle);
}

void SceneStageSelect::Init()
{
}

void SceneStageSelect::Update(Input& input)
{
	if (_frame < kUIControllInterval && _isUIMoveRight)
	{
		_frame++;
	}

	if (_frame > -kUIControllInterval && !_isUIMoveRight)
	{
		_frame--;
	}

	// UI移動演出中は操作が効かないようにする
	if (_frame == kUIControllInterval || _frame == -kUIControllInterval)
	{
		if (input.IsTriggered("right"))
		{
			// クリアしたステージ番号を反転したもの
			int clearedStageRev = static_cast<int>(Stages::Num) - 1 - static_cast<int>(_manager.GetClearedStage());
			if (clearedStageRev <= 0)	// 0になると最後のステージの次まで選べてしまうので0なら1にする
			{
				clearedStageRev = 1;
			}
			// 総ステージ数-反転ステージ番号=クリアしたステージ番号
			if (_selectIndex < _stageList.size() - clearedStageRev)
			{
				_selectIndex++;
				_isUIMoveRight = false;
				_frame = 0;
			}
			//printfDx("selectIndex:%d,stageList.size:%d,clearedStageRev:%d\n",_selectIndex,_stageList.size(), clearedStageRev);
		}
		if (input.IsTriggered("left"))
		{
			if (_selectIndex > 0)
			{
				_selectIndex--;
				_isUIMoveRight = true;
				_frame = 0;
			}
		}
		if (input.IsTriggered("decision"))
		{
			auto& stageName = _stageList[_selectIndex];
			_execTable[stageName]();
			return;
		}
		if (input.IsTriggered("back"))
		{
			_manager.ChangeSceneWithFade(std::make_shared<SceneTitle>(_manager), FadeState::NormalFadeIn);
			return;
		}
	}

#ifdef _DEBUG
	if (input.IsTriggered("select"))
	{
		_manager.ChangeScene(std::make_shared<DebugScene>(_manager));
	}
#endif
}

void SceneStageSelect::Draw()
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	// 背景
	DrawExtendGraph(0, 0, screenW, screenH, _bgHandle, false);

	// ステージUIの描画
	float progress = abs(static_cast<float>(_frame) / kUIControllInterval);
	int posX = _frame * kUIMoveScale;
	int sidePosX = kUIControllInterval * kUIMoveScale;
	if (_isUIMoveRight)
	{
		if (_selectIndex > 0)
		{
			DrawRotaGraph(screenW / 2 - sidePosX, screenH / 2, progress * kUIDrawScaleHalf, 0.0, _stageUIHandle, true);	// 進行方向の反対から現れるUI
		}
		if (_selectIndex < _stageList.size() - 1 - 1)
		{
			DrawRotaGraph(screenW / 2 + sidePosX, screenH / 2, kUIDrawScaleHalf - progress * kUIDrawScaleHalf, 0.0, _stageUIHandle, true);	// 進行方向で消えるUI
		}
		DrawRotaGraph(screenW / 2 - sidePosX + posX, screenH / 2, kUIDrawScaleHalf + progress * kUIDrawScaleHalf, 0.0, _stageUIHandle, true);	// 進行方向の反対から真ん中に向かうUI
	}
	else
	{
		if (_selectIndex < _stageList.size() - 1)
		{
			DrawRotaGraph(screenW / 2 + sidePosX, screenH / 2, progress * kUIDrawScaleHalf, 0.0, _stageUIHandle, true);	// 進行方向の反対から現れるUI
		}
		if (_selectIndex > 0 + 1)
		{
			DrawRotaGraph(screenW / 2 - sidePosX, screenH / 2, kUIDrawScaleHalf - progress * kUIDrawScaleHalf, 0.0, _stageUIHandle, true);	// 進行方向で消えるUI
		}
		DrawRotaGraph(screenW / 2 + sidePosX + posX, screenH / 2, kUIDrawScaleHalf + progress * kUIDrawScaleHalf, 0.0, _stageUIHandle, true);	// 進行方向の反対から真ん中に向かうUI
	}
	DrawRotaGraph(screenW / 2 + posX, screenH / 2, kUIDrawScale - progress * kUIDrawScaleHalf, 0.0, _stageUIHandle, true);	// 真ん中から進行方向に向かうUI
	

	// ステージ名の描画
	// UI移動中は描画しない
	if (_frame == kUIControllInterval || _frame == -kUIControllInterval)
	{
		if (_selectIndex > 0 && _selectIndex < _stageList.size() - 1)		// 最初のステージと最後のステージ以外を選択中の時は選択中、左、右のステージ名を表示
		{
			DrawFormatString(screenW / 2 - 400, screenH / 2, 0x888888, "< %s >", _stageList[_selectIndex - 1].c_str());	// 左のステージ名を表示
			DrawFormatString(screenW / 2, screenH / 2, 0xffffff, "< %s >", _stageList[_selectIndex].c_str());		// 選択中のステージ名を表示
			DrawFormatString(screenW / 2 + 400, screenH / 2, 0x888888, "< %s >", _stageList[_selectIndex + 1].c_str());	// 右のステージ名を表示
		}
		else if (_selectIndex == 0)	// 最初のステージを選択中のときは選択中と右のステージ名を表示
		{
			DrawFormatString(screenW / 2, screenH / 2, 0xffffff, "< %s >", _stageList[_selectIndex].c_str());		// 選択中のステージ名を表示
			DrawFormatString(screenW / 2 + 400, screenH / 2, 0x888888, "< %s >", _stageList[_selectIndex + 1].c_str());	// 右のステージ名を表示
		}
		else if (_selectIndex == _stageList.size() - 1)	// 最後のステージを選択中のときは選択中と左のステージ名を表示
		{
			DrawFormatString(screenW / 2 - 400, screenH / 2, 0x888888, "< %s >", _stageList[_selectIndex - 1].c_str());	// 左のステージ名を表示
			DrawFormatString(screenW / 2, screenH / 2, 0xffffff, "< %s >", _stageList[_selectIndex].c_str());		// 選択中のステージ名を表示
		}

		if (_selectIndex == static_cast<int>(_manager.GetClearedStage()))
		{
			DrawBox(screenW / 2 + 250 - 25,
				screenH / 2 - 200,
				screenW / 2+ 250 + 25,
				screenH / 2 + 200,
				0xff0000, true);
		}
	}

	DrawFormatString(0, 100, 0xffffff, "frame:%d", _frame);

#ifdef _DEBUG
	DrawString(0, 0, "SceneStageSelect",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "selectIndex:%d",_selectIndex);
#endif
}