#pragma once

/// <summary>
/// シーンの基底クラス
/// </summary>
class SceneBase
{
public:
	SceneBase();
	~SceneBase();

	virtual void Update();
	virtual void Draw();
};

