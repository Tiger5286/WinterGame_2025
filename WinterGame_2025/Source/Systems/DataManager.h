#pragma once

class DebugScene;

struct DataHeader
{
	char signature[4];	// "sdat"
	float version;
	int size;
};

struct SaveData
{
	int clearedStage = 0;
};

class DataManager
{
	//friend DebugScene;
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

