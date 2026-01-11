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
	// bgmをロード
	LoadSound("TitleBGM", "data/Sounds/BGM/TitleBGM.ogg", SoundType::BGM);
	LoadSound("StageSelectBGM", "data/Sounds/BGM/StageSelectBGM.ogg", SoundType::BGM);
	LoadSound("TutorialBgm", "data/Sounds/BGM/TutorialBGM.ogg", SoundType::BGM);
	LoadSound("StageBgm", "data/Sounds/BGM/StageBGM.ogg", SoundType::BGM);
	LoadSound("BossBgm", "data/Sounds/BGM/BossBGM.ogg", SoundType::BGM);
	LoadSound("ClearBgm", "data/Sounds/BGM/ClearBGM.ogg", SoundType::BGM);
	LoadSound("GameOverBgm", "data/Sounds/BGM/GameOverBGM.ogg", SoundType::BGM);

	// システム効果音をロード
	LoadSound("Select", "data/Sounds/System/Select.mp3", SoundType::SE);
	LoadSound("Decision", "data/Sounds/System/Decision.mp3", SoundType::SE);
	LoadSound("Cancel", "data/Sounds/System/Cancel.mp3", SoundType::SE);
	LoadSound("Beep", "data/Sounds/System/Beep.mp3", SoundType::SE);

	// プレイヤーの効果音
	LoadSound("Shot", "data/Sounds/Player/PlayerShot.mp3", SoundType::SE);
	LoadSound("ChargeShot", "data/Sounds/Player/PlayerChargeShot.mp3", SoundType::SE);
	LoadSound("Dash", "data/Sounds/Player/Dash.mp3", SoundType::SE);
	LoadSound("Jump", "data/Sounds/Player/Jump.mp3", SoundType::SE);
	LoadSound("Charge", "data/Sounds/Player/Charge.mp3", SoundType::SE);
	LoadSound("Charged", "data/Sounds/Player/Charged.mp3", SoundType::SE);
	LoadSound("WallJump", "data/Sounds/Player/WallJump.mp3", SoundType::SE);
	LoadSound("DashRecharge", "data/Sounds/Player/DashRecharge.mp3", SoundType::SE);
	LoadSound("Damage", "data/Sounds/Player/Damage.mp3", SoundType::SE);

	// アイテムの効果音
	LoadSound("Coin", "data/Sounds/Item/Coin.mp3", SoundType::SE);
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
			int newVolume = static_cast<int>(pair.second.fadeFrame / static_cast<float>(kFadeFrame) * targetVolume);
			if (pair.second.fadeState == SoundFadeState::None) newVolume = targetVolume;
			ChangeVolumeSoundMem(newVolume, pair.second.handle);
		}
	}
}

void SoundManager::LoadSound(const std::string& soundName, const std::string& filePath, SoundType type)
{
	// 同じ名前で登録しようとしたらエラー
	assert(_soundMap.find(soundName) == _soundMap.end() && "すでに登録されている音です");
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
	// 未登録の名前を渡すとエラー
	assert(_soundMap.find(soundName) != _soundMap.end() && "未登録の音です");
	// 登録された名前のハンドルの音を再生
	int handle = _soundMap[soundName].handle;
	PlaySoundMem(handle, isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);	// ループするかどうかを決める
	// フェードして再生するならフェード状態をフェードインにする
	if (isFade)
	{
		_soundMap[soundName].fadeState = SoundFadeState::FadeIn;
		_soundMap[soundName].fadeFrame = 0;
	}
	else	// フェードしないなら即再生
	{
		_soundMap[soundName].fadeState = SoundFadeState::None;
		_soundMap[soundName].fadeFrame = kFadeFrame;
	}
}

void SoundManager::StopSound(const std::string& soundName, bool isFade)
{
	// 未登録の名前を渡すとエラー
	assert(_soundMap.find(soundName) != _soundMap.end() && "未登録の音です");
	// フェードアウト状態にする
	_soundMap[soundName].fadeState = SoundFadeState::FadeOut;
	if (!isFade)
	{	// フェードしないならフェードアウトを即時終わらせる
		_soundMap[soundName].fadeState = SoundFadeState::None;
		_soundMap[soundName].fadeFrame = 0;
		StopSoundMem(_soundMap[soundName].handle);
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
		// 再生中なら止める
		if (CheckSoundMem(pair.second.handle))
		{
			StopSoundMem(pair.second.handle);
		}
		DeleteSoundMem(pair.second.handle);
	}
	// mapをクリア
	_soundMap.clear();
}

void SoundManager::DeleteSound(const std::string& soundName)
{
	// 未登録の名前を渡すとエラー
	assert(_soundMap.find(soundName) != _soundMap.end() && "未登録の音です");
	// 再生中なら止める
	if (CheckSoundMem(_soundMap[soundName].handle))
	{
		StopSoundMem(_soundMap[soundName].handle);
	}
	// 音をメモリから解放
	DeleteSoundMem(_soundMap[soundName].handle);
	// 登録されている名前を除外
	_soundMap.erase(soundName);
}

void SoundManager::ChangeVolume(SoundType type, int volume)
{
	// 音を範囲内に丸める
	if (volume < 0) volume = 0;
	if (volume > 255) volume = 255;

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
