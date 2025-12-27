#include "SoundManager.h"
#include "Dxlib.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	DeleteSoundAll();
}

void SoundManager::LoadSound(const std::string& soundName, const std::string& filePath, SoundType type)
{
	int soundHandle = LoadSoundMem(filePath.c_str());
	_soundMap[soundName].first = soundHandle;
	_soundMap[soundName].second = type;
}

void SoundManager::PlaySoundGame(const std::string& soundName, bool isLoop)
{
	int handle = _soundMap[soundName].first;
	PlaySoundMem(handle, isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
}

void SoundManager::DeleteSoundAll()
{
	for (const auto& pair : _soundMap)
	{
		DeleteSoundMem(pair.second.first);
	}
	_soundMap.clear();
}

void SoundManager::ChangeSoundVolume(const std::string& soundName, int volume)
{
	int handle = _soundMap[soundName].first;
	ChangeVolumeSoundMem(volume, handle);
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}
