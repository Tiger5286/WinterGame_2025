#include "Input.h"
#include "DxLib.h"

Input::Input() : inputData_{}, lastInputData_{}, inputTable_{}
{
	inputTable_["ok"] = { { PeripheralType::keyboard,KEY_INPUT_RETURN},
						  { PeripheralType::pad1,PAD_INPUT_L } };	// PADのSELECTボタン

	inputTable_["shot"] = { { PeripheralType::keyboard,KEY_INPUT_SPACE},
						  { PeripheralType::pad1,PAD_INPUT_C } };	// PADのXボタン

	inputTable_["up"] = { { PeripheralType::keyboard,KEY_INPUT_UP},
						{ PeripheralType::pad1,PAD_INPUT_UP } };
	inputTable_["down"] = { { PeripheralType::keyboard,KEY_INPUT_DOWN},
						  { PeripheralType::pad1,PAD_INPUT_DOWN } };
	inputTable_["left"] = { { PeripheralType::keyboard,KEY_INPUT_LEFT},
						  { PeripheralType::pad1,PAD_INPUT_LEFT } };
	inputTable_["right"] = { { PeripheralType::keyboard,KEY_INPUT_RIGHT},
						  { PeripheralType::pad1,PAD_INPUT_RIGHT } };
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
