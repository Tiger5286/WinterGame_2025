#pragma once
#include <array>
#include <map>
#include <string>
#include <vector>

/// <summary>
/// 周辺機器種別
/// </summary>
enum class PeripheralType
{
	keyboard,
	pad1
};

/// <summary>
/// 入力対応情報
/// </summary>
struct InputState
{
	PeripheralType type;	// 入力された機器の種類
	int id;	// 入力情報が入る(キーボードの場合はインデックス、パッドの場合はビット)
};

class Input
{
private:
	std::map<std::string, std::vector<InputState>>inputTable_; // イベント名と実際の入力の対応表
	std::map<std::string, bool> inputData_;	// 実際に入力されたかどうかのデータ
	std::map<std::string, bool> lastInputData_;	// 実際に入力されたかどうかのデータ
public:
	Input();

	/// <summary>
	/// これを毎フレーム呼び出すことで
	/// 入力情報が更新されます
	/// </summary>
	void Update();

	/// <summary>
	/// 特定のボタンが押されているか
	/// </summary>
	/// <param name="name">イベント名</param>
	/// <returns>押されている:true / 押されてない:false</returns>
	bool IsPressed(const char* name)const;

	/// <summary>
	/// 特定のボタンが今押されたか(押された瞬間にしか反応しない)
	/// </summary>
	/// <param name="name">イベント名</param>
	/// <returns>今押された瞬間:true / 押されてないor押しっぱなし:false</returns>
	bool IsTriggered(const char* name)const;

	/// <summary>
	/// 特定のボタンが今離されたか(離された瞬間にしか反応しない)
	/// </summary>
	/// <param name="name">イベント名</param>
	/// <returns>今離された瞬間:true / 押されているor離しっぱなし:false</returns>
	bool IsReleased(const char* name)const;
};

