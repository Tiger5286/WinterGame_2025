#include "GimmickManager.h"
#include "../Game.h"
#include "Dxlib.h"
#include <cassert>

#include "../GameObjects/Laser.h"

GimmickManager::GimmickManager(std::shared_ptr<Player> pPlayer):
	_pPlayer(pPlayer)
{
	_laserH = LoadGraph("data/Gimmicks/Laser.png");
	assert(_laserH != -1);
}

GimmickManager::~GimmickManager()
{
	DeleteGraph(_laserH);
}

void GimmickManager::Update()
{
	for (auto& gimmick : _pGimmicks)
	{
		gimmick->Update();
	}
}

void GimmickManager::Draw(Vector2 offset)
{
	for (auto& gimmick : _pGimmicks)
	{
		gimmick->Draw(offset);
	}
}

void GimmickManager::LoadGimmicks(const std::vector<uint16_t>& objectData, Size size)
{
	for (int w = 0; w < size.w; w++)
	{
		for (int h = 0; h < size.h; h++)
		{
			int index = h * size.w + w;
			Vector2 pos = Vector2(static_cast<float>(w), static_cast<float>(h));
			if (objectData[index] >= static_cast<int>(ObjectData::DownLaser2) &&
				objectData[index] <= static_cast<int>(ObjectData::DownLaser9))
			{
				int length = objectData[index] - 6;
				_pGimmicks.push_back(std::make_shared<Laser>(pos, _pPlayer, _laserH, length, true));
			}
			else if (objectData[index] >= static_cast<int>(ObjectData::UpLaser2) &&
				objectData[index] <= static_cast<int>(ObjectData::UpLaser9))
			{
				int length = objectData[index] - 14;
				_pGimmicks.push_back(std::make_shared<Laser>(pos, _pPlayer, _laserH, length, false));
			}
		}
	}
}