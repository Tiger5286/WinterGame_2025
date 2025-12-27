#include "SoundManager.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	constexpr int kFadeFrame = 60;
}

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	DeleteSoundAll();
}

void SoundManager::Update()
{
	for (auto& pair : _soundMap)
	{
		// 再生中の音がフェードイン・フェードアウト中かどうかを確認
		if (CheckSoundMem(pair.second.handle))
		{
			// フェードイン中ならフレームカウントを進める
			if (pair.second.fadeState == SoundFadeState::FadeIn)
			{
				// フレームカウントを進める
				if (pair.second.fadeFrame < kFadeFrame)
				{
					pair.second.fadeFrame++;
				}
				else
				{	// フェードイン完了
					pair.second.fadeState = SoundFadeState::None;
				}
			}	// フェードアウト中ならフレームカウントを戻す
			else if (pair.second.fadeState == SoundFadeState::FadeOut)
			{
				// フレームカウントを戻す
				if (pair.second.fadeFrame > 0)
				{
					pair.second.fadeFrame--;
				}
				else
				{	// フェードアウト完了
					pair.second.fadeState = SoundFadeState::None;
					StopSoundMem(pair.second.handle);	// 音を停止
				}
			}
			// フレームカウントに応じて音量を調整
			int targetVolume = pair.second.type == SoundType::BGM ? _bgmVolume : _seVolume;
			int newVolume = pair.second.fadeFrame / static_cast<float>(kFadeFrame) * targetVolume;
			ChangeVolumeSoundMem(newVolume, pair.second.handle);
		}
	}
}

void SoundManager::LoadSound(const std::string& soundName, const std::string& filePath, SoundType type)
{
	int soundHandle = LoadSoundMem(filePath.c_str());
	assert(soundHandle != -1 && "サウンドを正しくロードできませんでした。");
	_soundMap[soundName].handle = soundHandle;
	_soundMap[soundName].type = type;
	ChangeVolumeSoundMem(type == SoundType::BGM ? _bgmVolume : _seVolume, soundHandle);
}

void SoundManager::PlaySoundGame(const std::string& soundName, bool isLoop, bool isFade)
{
	int handle = _soundMap[soundName].handle;
	PlaySoundMem(handle, isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
	if (isFade) _soundMap[soundName].fadeState = SoundFadeState::FadeIn;
}

void SoundManager::StopSound(const std::string& soundName, bool isFade)
{
	if (isFade)
	{
		_soundMap[soundName].fadeState = SoundFadeState::FadeOut;
	}
	else
	{
		_soundMap[soundName].fadeState = SoundFadeState::FadeOut;
		_soundMap[soundName].fadeFrame = 0;
	}
}

void SoundManager::DeleteSoundAll()
{
	for (const auto& pair : _soundMap)
	{
		DeleteSoundMem(pair.second.handle);
	}
	_soundMap.clear();
}

void SoundManager::DeleteSound(const std::string& soundName)
{
	DeleteSoundMem(_soundMap[soundName].handle);
	_soundMap.erase(soundName);
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
		if (pair.second.type == type)
		{
			ChangeVolumeSoundMem(volume, pair.second.handle);
		}
	}
}

void SoundManager::SaveVolumeData(SaveData& data) const
{
	data.bgmVolume = _bgmVolume;
	data.seVolume = _seVolume;
}

void SoundManager::LoadVolumeData(const SaveData& data)
{
	_bgmVolume = data.bgmVolume;
	_seVolume = data.seVolume;
	ChangeVolume(SoundType::BGM, _bgmVolume);
	ChangeVolume(SoundType::SE, _seVolume);
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}
