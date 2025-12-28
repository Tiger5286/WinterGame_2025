#include "SoundManager.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	// 音がフェードする時間(フレーム)
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
	// 音のフェードイン、アウト処理
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
	// 音を読み込み
	int soundHandle = LoadSoundMem(filePath.c_str());
	assert(soundHandle != -1 && "サウンドを正しくロードできませんでした。");
	// mapにハンドルと音の種類を登録
	_soundMap[soundName].handle = soundHandle;
	_soundMap[soundName].type = type;
	// 音の種類に応じて音量を調節
	ChangeVolumeSoundMem(type == SoundType::BGM ? _bgmVolume : _seVolume, soundHandle);
}

void SoundManager::PlaySoundGame(const std::string& soundName, bool isLoop, bool isFade)
{
	// 登録された名前のハンドルの音を再生
	int handle = _soundMap[soundName].handle;
	PlaySoundMem(handle, isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);	// ループするかどうかを決める
	// フェードして再生するならフェード状態をフェードインにする
	if (isFade) _soundMap[soundName].fadeState = SoundFadeState::FadeIn;
}

void SoundManager::StopSound(const std::string& soundName, bool isFade)
{
	// フェードアウト状態にする
	_soundMap[soundName].fadeState = SoundFadeState::FadeOut;
	if (!isFade)
	{	// フェードしないならフェードアウトを即時終わらせる
		_soundMap[soundName].fadeFrame = 0;
	}
}

void SoundManager::StopSoundAll(bool isFade)
{
	// 全ての登録されている音でStopSoundを実行
	for (auto& pair : _soundMap)
	{
		StopSound(pair.first, isFade);
	}
}

void SoundManager::DeleteSoundAll()
{
	// 全ての音をメモリから解放
	for (const auto& pair : _soundMap)
	{
		DeleteSoundMem(pair.second.handle);
	}
	// mapをクリア
	_soundMap.clear();
}

void SoundManager::DeleteSound(const std::string& soundName)
{
	// 音をメモリから解放
	DeleteSoundMem(_soundMap[soundName].handle);
	// 登録されている名前を除外
	_soundMap.erase(soundName);
}

void SoundManager::ChangeVolume(SoundType type, int volume)
{
	// 音の種類によって調節する音を変える
	switch (type)
	{
	case SoundType::BGM:
		_bgmVolume = volume;
		break;
	case SoundType::SE:
		_seVolume = volume;
		break;
	}
	// 音の種類ごとに音量を調節
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
	// セーブデータに音量を保存
	data.bgmVolume = _bgmVolume;
	data.seVolume = _seVolume;
}

void SoundManager::LoadVolumeData(const SaveData& data)
{
	// セーブデータから音量を取り込み
	_bgmVolume = data.bgmVolume;
	_seVolume = data.seVolume;
	// 取り込んだ音量を適用
	ChangeVolume(SoundType::BGM, _bgmVolume);
	ChangeVolume(SoundType::SE, _seVolume);
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}
