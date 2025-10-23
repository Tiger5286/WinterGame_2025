#include "SceneMain.h"
#include "DxLib.h"
#include "Application.h"
#include "input.h"

SceneMain::SceneMain() :
	m_frameCount(0)
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Update()
{
	m_frameCount++;
	
}

void SceneMain::Draw()
{
	DrawString(0,0,"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "FRAME:%d", m_frameCount);
}