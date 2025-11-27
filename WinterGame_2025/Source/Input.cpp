#include "Input.h"
#include "DxLib.h"
#include "Game.h"

Input::Input() : _inputData{}, _lastInputData{}, _inputTable{}
{
	_inputTable["start"] = { { PeripheralType::keyboard,KEY_INPUT_P},
						  { PeripheralType::pad1,GlobalConstants::kPadInputStart } };
	_inputTable["select"] = { { PeripheralType::keyboard,KEY_INPUT_RETURN},
						  { PeripheralType::pad1,GlobalConstants::kPadInputSelect } };

	_inputTable["decision"] = { { PeripheralType::keyboard,KEY_INPUT_Z},
						  { PeripheralType::pad1,GlobalConstants::kPadInputA } };
	_inputTable["back"] = { { PeripheralType::keyboard,KEY_INPUT_X},
						  { PeripheralType::pad1,GlobalConstants::kPadInputB } };

	_inputTable["jump"] = { { PeripheralType::keyboard,KEY_INPUT_SPACE},
						  { PeripheralType::pad1,GlobalConstants::kPadInputA } };
	_inputTable["shot"] = { { PeripheralType::keyboard,KEY_INPUT_Z},
						  { PeripheralType::pad1,GlobalConstants::kPadInputX } };
	_inputTable["dash"] = { { PeripheralType::keyboard,KEY_INPUT_X},
						  { PeripheralType::pad1,GlobalConstants::kPadInputB } };


	_inputTable["up"] = { { PeripheralType::keyboard,KEY_INPUT_UP},
						{ PeripheralType::pad1,GlobalConstants::kPadInputUp } };
	_inputTable["down"] = { { PeripheralType::keyboard,KEY_INPUT_DOWN},
						  { PeripheralType::pad1,GlobalConstants::kPadInputDown } };
	_inputTable["left"] = { { PeripheralType::keyboard,KEY_INPUT_LEFT},
						  { PeripheralType::pad1,GlobalConstants::kPadInputLeft } };
	_inputTable["right"] = { { PeripheralType::keyboard,KEY_INPUT_RIGHT},
						  { PeripheralType::pad1,GlobalConstants::kPadInputRight } };
	// あらかじめ枠を開けておく
	for (const auto& inputInfo : _inputTable)
	{
		_inputData[inputInfo.first] = false;
		_lastInputData[inputInfo.first] = false;
	}
}

void Input::Update()
{
	// まず現在の入力情報を取得
	char keyState[256];
	GetHitKeyStateAll(keyState);
	int padState = GetJoypadInputState(DX_INPUT_PAD1);
	_lastInputData = _inputData;
	// すべての入力イベントをチェックします
	for (const auto& inputInfo : _inputTable)
	{
		auto& input = _inputData[inputInfo.first];
		for (const auto& state : inputInfo.second)
		{
			switch (state.type)
			{
			case PeripheralType::keyboard:
				input = keyState[state.id];
				break;
			case PeripheralType::pad1:
				input = (padState & state.id);
				break;
			}
			if (input)	// 必須！
			{
				break;
			}
		}
	}
}

bool Input::IsPressed(const char* name) const
{
	return _inputData.at(name);
}

bool Input::IsTriggered(const char* name) const
{
	return _inputData.at(name) && !_lastInputData.at(name);
}

bool Input::IsReleased(const char* name) const
{
	return !_inputData.at(name) && _lastInputData.at(name);
}
