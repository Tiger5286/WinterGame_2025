#include "SceneManager.h"
#include "SceneBase.h"
#include "../Systems/Input.h"
#include "../Systems/Fade.h"
#include "Dxlib.h"
#include "../Systems/DataManager.h"

SceneManager::SceneManager():
	_pFade(std::make_shared<Fade>()),
	_pNextScene(nullptr),
	_nextFadeType(FadeState::NormalFadeIn),
	_stopFrame(0)
{
	_pDataManager = std::make_shared<DataManager>();
	_pDataManager->Load();
	_clearedStage = static_cast<Stages>(_pDataManager->GetSaveData().clearedStage);
}

SceneManager::~SceneManager()
{
	SaveData data;
	data.clearedStage = static_cast<int>(_clearedStage);
	_pDataManager->SetSaveData(data);
	_pDataManager->Save();
}

void SceneManager::Init()
{
	_pScenes.back()->Init();
}

void SceneManager::Update(Input input)
{
	// 停止フレームが設定されている場合は更新をスキップする
	if (_stopFrame > 0)
	{
		_stopFrame--;
		return;
	}

	_pFade->Update();
	// フェード中はシーンの更新を行わない
	if (_pFade->GetState() == FadeState::NoFade)
	{
		_pScenes.back()->Update(input);
	}

	// 次のシーンが決まっているときにフェードアウトが終わったらシーンを変える
	if (_pNextScene != nullptr)
	{
		if (_pFade->GetIsFadeEnd())
		{
			ChangeScene(_pNextScene,_nextFadeType);
			_pNextScene = nullptr;
		}
	}
}

void SceneManager::Draw()
{
	for (auto& scene : _pScenes)
	{
		scene->Draw();
	}
	_pFade->Draw();
	DrawFormatString(0, 100, 0xffffff, "clearedStage:%d", static_cast<int>(_clearedStage));
}

void SceneManager::ResetScene(std::shared_ptr<SceneBase> pScene)
{
	_pScenes.clear();
	_pScenes.push_back(pScene);
	_pScenes.back()->Init();
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> pScene)
{
	_pScenes.push_back(pScene);
	_pScenes.back()->Init();
}

void SceneManager::PopScene()
{
	_pScenes.pop_back();
}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> scene,FadeState fadeType)
{
	// もしシーンがなければ追加、あれば置き換え
	if (_pScenes.empty())
	{
		_pScenes.push_back(scene);
		_pScenes.back()->Init();
	}
	else
	{
		_pScenes.back() = scene;
		_pScenes.back()->Init();
	}
	// シーン変更時にフェードインを開始する
	if (fadeType == FadeState::NormalFadeIn)
	{
		_pFade->StartFadeIn();
	}
	else if (fadeType == FadeState::CircleFadeIn)
	{
		_pFade->StartCircleFadeIn();
	}
}

void SceneManager::ChangeSceneWithFade(std::shared_ptr<SceneBase> scene, FadeState nextFadeType, FadeState fadeType)
{
	if (fadeType == FadeState::NormalFadeOut)
	{
		_pFade->StartFadeOut();
	}
	else if (fadeType == FadeState::CircleFadeOut)
	{
		_pFade->StartCircleFadeOut();
	}
	_nextFadeType = nextFadeType;
	_pNextScene = scene;
}

void SceneManager::CheckClearedStage(Stages clearedStage)
{
	// クリアしたステージが登録済みのステージより前なら何もしない
	if (static_cast<int>(clearedStage) <= static_cast<int>(_clearedStage)) return;

	for (int i = 0; i < static_cast<int>(Stages::Num); i++)
	{
		// クリアしたステージを登録する
		if (clearedStage == static_cast<Stages>(i))
		{
			_clearedStage = clearedStage;
			return;	// ステージを登録した時点でループを終わる
		}
	}
}