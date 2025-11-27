#include "SceneManager.h"
#include "SceneBase.h"
#include "../Input.h"
#include "../Fade.h"

SceneManager::SceneManager():
	_pScene(nullptr),
	_pFade(std::make_shared<Fade>()),
	_pNextScene(nullptr),
	_nextFadeType(FadeState::NormalFadeIn)
{
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
			ChangeScene(_pNextScene,_nextFadeType);
			_pNextScene = nullptr;
		}
	}
}

void SceneManager::Draw()
{
	_pScene->Draw();
	_pFade->Draw();
}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> scene,FadeState fadeType)
{
	_pScene = scene;
	_pScene->Init();
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
