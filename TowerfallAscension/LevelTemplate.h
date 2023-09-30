#pragma once
#include "utils.h"
#include <vector>
#include "Enemy.h"
#include "Upgrade.h"
#include "Scenery.h"
#include "SoundManager.h"

class Level;

struct EnemyInfo
{
	Enemy::EnemyType enemyType;
	Point2f spawnPoint;
	float spawnDelay;
	int wave;
};

struct LevelInfo
{
	std::string levelName;
	Point2f playerSpawn;	
	std::vector<std::vector<Point2f>> vertices;
	Point2f mapLocation;
	std::vector<Point2f> torchLocations;
	Scenery::SceneryType sceneryType;
	SoundManager::Music music;
	std::vector<Point2f> floatingPath;
};

struct ChestInfo
{
	Upgrade::UpgradeType upgradeType;
	Point2f chestSpawn;
};

struct PlayerInfo
{
		int lives;
		int arrows;	
};

class LevelTemplate final
{
public:
	LevelTemplate(const LevelInfo& level, const PlayerInfo& player, const std::vector<ChestInfo>& chestSpawns, const std::vector<EnemyInfo>& enemies);
	~LevelTemplate() = default;
	LevelTemplate(const LevelTemplate& c) = default;
	LevelTemplate& operator=(const LevelTemplate& rhs) = default;
	LevelTemplate(LevelTemplate&& c) noexcept = default;
	LevelTemplate& operator=(LevelTemplate && rhs) = default;

	
	PlayerInfo m_Player;
	const LevelInfo m_Level;
	const std::vector<ChestInfo> m_Chests;
	const std::vector<EnemyInfo> m_Enemies;
	
	
};

