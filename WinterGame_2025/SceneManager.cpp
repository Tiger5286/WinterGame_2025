#include "SceneManager.h"
#include "SceneBase.h"
#include "Input.h"

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> scene)
{
	_scene = scene;
	_scene->Init();
}

void SceneManager::Init()
{
	_scene->Init();
}

void SceneManager::Update(Input input)
{
	_scene->Update(input);
}

void SceneManager::Draw()
{
	_scene->Draw();
}