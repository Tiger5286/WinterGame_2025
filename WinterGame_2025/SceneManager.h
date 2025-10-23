#pragma once
#include <memory>

class SceneBase;

/// <summary>
/// 内部にシーンを保持し、切り替えなどを行うクラス
/// </summary>
class SceneManager
{
private:
	std::shared_ptr<SceneBase> m_scene;
public:
	void ChangeScene(std::shared_ptr<SceneBase> scene);
	void Update();
	void Draw();
};

