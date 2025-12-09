#pragma once
#include "../Game.h"

class DebugScene;

class DataManager
{
public:
	DataManager();
	~DataManager();

	void SetSaveData(const SaveData& data) { _saveData = data; }
	SaveData GetSaveData() const { return _saveData; }
	void DeleteSaveData() { _saveData = SaveData(); }

	void Save();
	void Load();

private:
	SaveData _saveData;
};