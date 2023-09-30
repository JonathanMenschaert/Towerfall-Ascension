#pragma once
#include <vector>
#include "LevelTemplate.h"

struct SoundSettings
{
	int sfxVolume{};
	int musicVolume{};
};

class ResourceManager;
class GameData final
{
public:
	GameData(const std::string& rootFile);
	~GameData() = default;
	GameData(const GameData& c) = delete;
	GameData& operator=(const GameData& rhs) = delete;
	GameData(GameData&& c) = delete;
	GameData& operator=(GameData&& rhs) = delete;

	const LevelTemplate GetLevelTemplateById(int idx) const;
	const std::vector<Vector2f> GetMapPointers() const;
	const std::vector<std::string> GetLevelNames() const;
	const SoundSettings& GetSoundSettings() const;
	void SaveOptions(const ResourceManager* resources);

private:
	SoundSettings m_SoundSettings;
	std::vector<LevelTemplate> m_LevelTemplates;

	const std::string m_RootFile;
	std::vector<std::string> m_LevelFiles;
	std::string m_OptionsFile;

	void ReadRootFile();
	void ReadRootElement(const std::string& attribute);
	std::string GetAttributeValue(const std::string& attrName, const std::string& element) const;
	void AddLevelPath(const std::string& attribute);
	void SetOptionsPath(const std::string& attribute);
	
	
	void CreateLevelTemplatesFromFiles();
	void CreateOptionsTemplatesFromFile();
	Point2f ToPoint2f(const std::string& point2fStr) const;
	int ToInteger(const std::string& intStr) const;
	float ToFloat(const std::string& floatStr) const;
	std::vector<Point2f> ToPoint2fVector(const std::string& point2fStr) const;

	LevelInfo ExtractLevelInfo(const std::string& attribute);
	ChestInfo ExtractChestInfo(const std::string& attribute);
	EnemyInfo ExtractEnemyInfo(const std::string& attribute);
	PlayerInfo ExtractPlayerInfo(const std::string& attribute);
	SoundSettings ExtractSoundInfo(const std::string& attribute);

};

