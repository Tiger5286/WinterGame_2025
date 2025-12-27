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
	ChangeVolumeSoundMem(type == SoundType::BGM ? _bgmVolume : _seVolume, soundHandle);
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

void SoundManager::ChangeVolume(SoundType type, int volume)
{
	switch (type)
	{
	case SoundType::BGM:
		_bgmVolume = volume;
		break;
	case SoundType::SE:
		_seVolume = volume;
		break;
	}

	for (const auto& pair : _soundMap)
	{
		if (pair.second.second == type)
		{
			ChangeVolumeSoundMem(volume, pair.second.first);
		}
	}
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}
