#include "pch.h"
#include "SpawnTimer.h"
#include "Enemy.h"

SpawnTimer::SpawnTimer(Enemy* enemy, int wave, float spawnDelay)
	:m_pEnemy{enemy}
	,m_Wave{wave}
	,m_SpawnDelay{spawnDelay}
{
}

SpawnTimer::~SpawnTimer()
{
	delete m_pEnemy;
	m_pEnemy = nullptr;
}

void SpawnTimer::Update(float elapsedSec, int wave)
{
	if (m_Wave == wave)
	{
		m_SpawnDelay -= elapsedSec;
	}
}

bool SpawnTimer::CanSpawn(int wave) const
{
	if (m_Wave == wave && m_SpawnDelay <= 0.f)
	{
		return true;
	}
	return false;
}

Enemy* SpawnTimer::GetEnemy()
{
	Enemy* pEnemy = m_pEnemy;
	m_pEnemy = nullptr;
	pEnemy->SetSpawnerActive();
	return pEnemy;
}

int SpawnTimer::GetSpawnWave() const
{
	return m_Wave;
}
