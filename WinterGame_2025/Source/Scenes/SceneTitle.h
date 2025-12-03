#pragma once
#include "SceneBase.h"
class SceneTitle : public SceneBase
{
public:
	SceneTitle(SceneManager& manager);
	~SceneTitle();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
private:
	int _frame = 0;
	// ‰æ‘œƒnƒ“ƒhƒ‹
	int _bgHandle = -1;
	int _titleHandle = -1;
};

