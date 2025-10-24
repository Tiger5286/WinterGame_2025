#include "Input.h"
#include "DxLib.h"
#include "Game.h"

Input::Input() : inputData_{}, lastInputData_{}, inputTable_{}
{
	inputTable_["start"] = { { PeripheralType::keyboard,KEY_INPUT_RETURN},
						  { PeripheralType::pad1,GlobalConstants::kPadStart } };

	inputTable_["jump"] = { { PeripheralType::keyboard,KEY_INPUT_SPACE},
						  { PeripheralType::pad1,GlobalConstants::kPadA } };
	inputTable_["shot"] = { { PeripheralType::keyboard,KEY_INPUT_SPACE},
						  { PeripheralType::pad1,GlobalConstants::kPadX } };
	inputTable_["dash"] = { { PeripheralType::keyboard,KEY_INPUT_SPACE},
						  { PeripheralType::pad1,GlobalConstants::kPadB } };


	inputTable_["up"] = { { PeripheralType::keyboard,KEY_INPUT_UP},
						{ PeripheralType::pad1,GlobalConstants::kPadUp } };
	inputTable_["down"] = { { PeripheralType::keyboard,KEY_INPUT_DOWN},
						  { PeripheralType::pad1,GlobalConstants::kPadDown } };
	inputTable_["left"] = { { PeripheralType::keyboard,KEY_INPUT_LEFT},
						  { PeripheralType::pad1,GlobalConstants::kPadLeft } };
	inputTable_["right"] = { { PeripheralType::keyboard,KEY_INPUT_RIGHT},
						  { PeripheralType::pad1,GlobalConstants::kPadRight } };
	// あらかじめ枠を開けておく
	for (const auto& inputInfo : inputTable_)
	{
		inputData_[inputInfo.first] = false;
		lastInputData_[inputInfo.first] = false;
	}
}

void Input::Update()
{
	// まず現在の入力情報を取得
	char keyState[256];
	GetHitKeyStateAll(keyState);
	int padState = GetJoypadInputState(DX_INPUT_PAD1);
	lastInputData_ = inputData_;
	// すべての入力イベントをチェックします
	for (const auto& inputInfo : inputTable_)
	{
		auto& input = inputData_[inputInfo.first];
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
	return inputData_.at(name);
}

bool Input::IsTriggered(const char* name) const
{
	return inputData_.at(name) && !lastInputData_.at(name);
}
