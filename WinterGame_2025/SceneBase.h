#pragma once

class Input;

/// <summary>
/// シーンの基底クラス
/// </summary>
class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase();

	virtual void Init() = 0;
	virtual void Update(Input input) = 0;
	virtual void Draw() = 0;
};

