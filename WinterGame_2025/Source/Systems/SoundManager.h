#pragma once
#include <map>
#include <string>

enum class SoundType
{
	BGM,
	SE
};

class SoundManager
{
public:
	static SoundManager& GetInstance();
	~SoundManager();

	/// <summary>
	/// 音声ファイルを読み込み、登録する
	/// </summary>
	/// <param name="soundName">登録する名前</param>
	/// <param name="filePath">ファイル名</param>
	/// <param name="type">音の種類</param>
	void LoadSound(const std::string& soundName, const std::string& filePath,SoundType type);

	/// <summary>
	/// 読み込んだ音声ファイルを再生する
	/// </summary>
	/// <param name="soundName">登録した名前</param>
	/// <param name="isLoop">ループするかどうか</param>
	void PlaySoundGame(const std::string& soundName, bool isLoop = false);	// PlaySoundにしたかったけど何かと被ってエラー吐くので仕方なくPlaySoundGameに

	/// <summary>
	/// 読み込んだ音声ファイルを全て開放し、登録を解除する
	/// </summary>
	void DeleteSoundAll();

	void ChangeSoundVolume(const std::string& soundName, int volume);

private:
	SoundManager();
	SoundManager(const SoundManager& sm) = delete;
	void operator=(const SoundManager& sm) = delete;

	std::map<std::string, std::pair<int,SoundType>> _soundMap;
};

