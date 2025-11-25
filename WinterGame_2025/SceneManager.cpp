#include "SceneManager.h"
#include "SceneBase.h"
#include "Input.h"
#include "Fade.h"

SceneManager::SceneManager()
{
	_pScene = nullptr;
	_pFade = std::make_shared<Fade>();
}

void SceneManager::Init()
{
	_pScene->Init();
}

void SceneManager::Update(Input input)
{
	_pFade->Update();
	// フェード中はシーンの更新を行わない
	if (_pFade->GetState() == FadeState::NoFade)
	{
		_pScene->Update(input);
	}

	// 次のシーンが決まっているときにフェードアウトが終わったらシーンを変える
	if (_pNextScene != nullptr)
	{
		if (_pFade->GetIsFadeEnd())
		{
			ChangeScene(_pNextScene);
			_pNextScene = nullptr;
		}
	}
}

void SceneManager::Draw()
{
	_pScene->Draw();
	_pFade->Draw();
}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> scene)
{
	_pScene = scene;
	_pScene->Init();
	// シーン変更時にフェードインを開始する
	_pFade->StartFadeIn();
}

void SceneManager::ChangeSceneWithFadeOut(std::shared_ptr<SceneBase> scene)
{
	_pFade->StartFadeOut();
	_pNextScene = scene;
}
