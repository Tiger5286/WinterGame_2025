#include "Input.h"
#include "DxLib.h"
#include "Game.h"

Input::Input() : inputData_{}, lastInputData_{}, inputTable_{}
{
	inputTable_["start"] = { { PeripheralType::keyboard,KEY_INPUT_P},
						  { PeripheralType::pad1,GlobalConstants::PAD_START } };
	inputTable_["select"] = { { PeripheralType::keyboard,KEY_INPUT_RETURN},
						  { PeripheralType::pad1,GlobalConstants::PAD_SELECT } };

	inputTable_["decision"] = { { PeripheralType::keyboard,KEY_INPUT_Z},
						  { PeripheralType::pad1,GlobalConstants::PAD_A } };
	inputTable_["back"] = { { PeripheralType::keyboard,KEY_INPUT_X},
						  { PeripheralType::pad1,GlobalConstants::PAD_B } };

	inputTable_["jump"] = { { PeripheralType::keyboard,KEY_INPUT_SPACE},
						  { PeripheralType::pad1,GlobalConstants::PAD_A } };
	inputTable_["shot"] = { { PeripheralType::keyboard,KEY_INPUT_Z},
						  { PeripheralType::pad1,GlobalConstants::PAD_X } };
	inputTable_["dash"] = { { PeripheralType::keyboard,KEY_INPUT_X},
						  { PeripheralType::pad1,GlobalConstants::PAD_B } };


	inputTable_["up"] = { { PeripheralType::keyboard,KEY_INPUT_UP},
						{ PeripheralType::pad1,GlobalConstants::PAD_UP } };
	inputTable_["down"] = { { PeripheralType::keyboard,KEY_INPUT_DOWN},
						  { PeripheralType::pad1,GlobalConstants::PAD_DOWN } };
	inputTable_["left"] = { { PeripheralType::keyboard,KEY_INPUT_LEFT},
						  { PeripheralType::pad1,GlobalConstants::PAD_LEFT } };
	inputTable_["right"] = { { PeripheralType::keyboard,KEY_INPUT_RIGHT},
						  { PeripheralType::pad1,GlobalConstants::PAD_RIGHT } };
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

bool Input::IsReleased(const char* name) const
{
	return !inputData_.at(name) && lastInputData_.at(name);
}
