#pragma once
#include "SceneBase.h"

class SceneMain : public SceneBase
{
public:
	SceneMain();
	~SceneMain();

	void Update() override;
	void Draw() override;
private:
	int m_frameCount;
};

