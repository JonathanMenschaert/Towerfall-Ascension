#include "pch.h"
#include "GameData.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "SVGParser.h"
#include "ResourceManager.h"
#include "SoundManager.h"

GameData::GameData(const std::string& rootFile)
    :m_RootFile{rootFile}
{
    ReadRootFile();
	CreateLevelTemplatesFromFiles();
	CreateOptionsTemplatesFromFile();
}

const LevelTemplate GameData::GetLevelTemplateById(int idx) const
{
    return m_LevelTemplates[idx];
}

const std::vector<Vector2f> GameData::GetMapPointers() const
{
	std::vector<Vector2f> pointers;
	for (size_t i{}; i < m_LevelTemplates.size(); ++i)
	{
		pointers.push_back(Vector2f{ m_LevelTemplates[i].m_Level.mapLocation });
	}
	return pointers;
}

const std::vector<std::string> GameData::GetLevelNames() const
{
	std::vector<std::string> names;
	for (size_t i{}; i < m_LevelTemplates.size(); ++i)
	{
		names.push_back(m_LevelTemplates[i].m_Level.levelName );
	}
	return names;
}

const SoundSettings& GameData::GetSoundSettings() const
{
	return m_SoundSettings;
}

void GameData::SaveOptions(const ResourceManager* resources)
{
	std::ofstream os{ m_OptionsFile };

	os << "<SoundOptions\n" << resources->GetSoundManager()->ToString() << "/>";

}

void GameData::ReadRootFile()
{
	std::ifstream is{ m_RootFile };
	std::string element{};
	while (std::getline(is, element, '>'))
	{
		ReadRootElement(element);		
	}
}

void GameData::ReadRootElement(const std::string& attribute)
{
	if (attribute.find("LevelPath") != std::string::npos)
	{
		AddLevelPath(attribute);
	}
	else if (attribute.find("Options") != std::string::npos)
	{
		SetOptionsPath(attribute);
	}
}

std::string GameData::GetAttributeValue(const std::string& attrName, const std::string& element) const
{

	size_t startIdxAttr = element.find(attrName + "=");
	size_t startIdxElement = element.find("\"", startIdxAttr) + 1;
	size_t endIdxElement = element.find("\"", startIdxElement);	
	return std::string{ element.substr(startIdxElement, endIdxElement - startIdxElement) };
}

void GameData::AddLevelPath(const std::string& attribute)
{
	m_LevelFiles.push_back(GetAttributeValue("Path", attribute));
}

void GameData::SetOptionsPath(const std::string& attribute)
{
	m_OptionsFile = GetAttributeValue("Path", attribute);
}

void GameData::CreateLevelTemplatesFromFiles()
{
	for (size_t i{}; i < m_LevelFiles.size(); ++i)
	{
		std::ifstream is{ m_LevelFiles[i] };
		std::string element{};
		LevelInfo levelInfo{};
		std::vector < ChestInfo> chests{};
		std::vector<EnemyInfo> enemies{};
		PlayerInfo playerInfo{};

		while (std::getline(is, element, '>'))
		{
			if (element.find("Enemy") != std::string::npos)
			{
				enemies.push_back(ExtractEnemyInfo(element));
			}
			else if (element.find("Chest") != std::string::npos)
			{
				chests.push_back(ExtractChestInfo(element));
			}			
			else if (element.find("Level") != std::string::npos)
			{
				levelInfo = ExtractLevelInfo(element);
			}
			else if (element.find("Player") != std::string::npos)
			{
				playerInfo = ExtractPlayerInfo(element);
			}
		}

		m_LevelTemplates.push_back(LevelTemplate(levelInfo, playerInfo, chests, enemies));
	}
}

void GameData::CreateOptionsTemplatesFromFile()
{
	std::ifstream is{ m_OptionsFile };
	std::string element{};

	while (std::getline(is, element, '>'))
	{
		if (element.find("SoundOptions") != std::string::npos)
		{
			m_SoundSettings = ExtractSoundInfo(element);
		}
	}
}

LevelInfo GameData::ExtractLevelInfo(const std::string& attribute)
{	
	LevelInfo levelInfo{};
	levelInfo.levelName = GetAttributeValue("Name", attribute);
	levelInfo.playerSpawn = ToPoint2f(GetAttributeValue("PlayerSpawn", attribute));
	SVGParser::GetVerticesFromSvgFile(GetAttributeValue("VerticesPath", attribute), levelInfo.vertices);
	levelInfo.mapLocation = ToPoint2f(GetAttributeValue("MapLocation", attribute));
	levelInfo.torchLocations = ToPoint2fVector(GetAttributeValue("TorchLocations", attribute));	
	levelInfo.sceneryType = Scenery::SceneryType(ToInteger(GetAttributeValue("SceneryType", attribute)));
	levelInfo.floatingPath = ToPoint2fVector(GetAttributeValue("FloatingPath", attribute));
	levelInfo.music = SoundManager::Music(ToInteger(GetAttributeValue("MusicTrack", attribute)));
	return levelInfo;
}

ChestInfo GameData::ExtractChestInfo(const std::string& attribute)
{
	ChestInfo chestInfo{};
	chestInfo.upgradeType = Upgrade::UpgradeType(ToInteger(GetAttributeValue("UpgradeType", attribute)));
	chestInfo.chestSpawn = ToPoint2f(GetAttributeValue("ChestSpawn", attribute));
	return chestInfo;
}

EnemyInfo GameData::ExtractEnemyInfo(const std::string& attribute)
{
	EnemyInfo enemyInfo{};
	enemyInfo.enemyType = Enemy::EnemyType(ToInteger(GetAttributeValue("EnemyType", attribute)));
	enemyInfo.spawnPoint = ToPoint2f(GetAttributeValue("EnemySpawn", attribute));
	enemyInfo.spawnDelay = ToFloat(GetAttributeValue("SpawnDelay", attribute));
	enemyInfo.wave = ToInteger(GetAttributeValue("Wave", attribute));
	return enemyInfo;
}

PlayerInfo GameData::ExtractPlayerInfo(const std::string& attribute)
{
	PlayerInfo playerInfo{};
	playerInfo.lives = ToInteger(GetAttributeValue("Lives", attribute));
	playerInfo.arrows = ToInteger(GetAttributeValue("Arrows", attribute));
	return playerInfo;
}

SoundSettings GameData::ExtractSoundInfo(const std::string& attribute)
{
	SoundSettings settings{};
	settings.sfxVolume = ToInteger(GetAttributeValue("SfxVolume", attribute));
	settings.musicVolume = ToInteger(GetAttributeValue("MusicVolume", attribute));
	return settings;
}

Point2f GameData::ToPoint2f(const std::string& point2fStr) const
{
	Point2f point;

	std::stringstream ss;
	ss.str(point2fStr);

	std::string coordinate{};
	std::getline(ss, coordinate, ',');
	point.x = std::stof(coordinate);
	std::getline(ss, coordinate, ',');
	point.y = std::stof(coordinate);
	return point;
}

int GameData::ToInteger(const std::string& intStr) const
{
	return std::stoi(intStr);
}

float GameData::ToFloat(const std::string& floatStr) const
{
	return std::stof(floatStr);
}

std::vector<Point2f> GameData::ToPoint2fVector(const std::string& point2fStr) const
{
	std::stringstream ss;
	ss.str(point2fStr);

	std::vector<Point2f> points;
	std::string element{};

	while (std::getline(ss, element, ';'))
	{
		points.push_back(ToPoint2f(element));
	}
	return points;
}
