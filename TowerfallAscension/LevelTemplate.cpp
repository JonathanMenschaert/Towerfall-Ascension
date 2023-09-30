#include "pch.h"
#include "LevelTemplate.h"

LevelTemplate::LevelTemplate(const LevelInfo& level, const PlayerInfo& player, const std::vector<ChestInfo>& chestSpawns, const std::vector<EnemyInfo>& enemies)
	:m_Level{level}
	,m_Chests{chestSpawns}
	,m_Enemies{enemies}
	,m_Player{player}
{
}
