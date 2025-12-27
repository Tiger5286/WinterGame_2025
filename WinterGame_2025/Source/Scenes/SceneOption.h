#pragma once
#include "SceneBase.h"
class SceneOption :
    public SceneBase
{
public:
	SceneOption(SceneManager& manager);
	~SceneOption();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
};

