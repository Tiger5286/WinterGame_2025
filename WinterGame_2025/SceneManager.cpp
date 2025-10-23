#include "SceneManager.h"
#include "SceneBase.h"

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> scene)
{
	m_scene = scene;
}

void SceneManager::Update()
{
	m_scene->Update();
}

void SceneManager::Draw()
{
	m_scene->Draw();
}