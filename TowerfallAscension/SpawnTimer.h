#pragma once

class Enemy;
class SpawnTimer final
{
public:
	SpawnTimer(Enemy* enemy, int wave, float spawnDelay);
	~SpawnTimer();

	SpawnTimer(const SpawnTimer& c) = delete;
	SpawnTimer& operator=(const SpawnTimer& rhs) = delete;
	SpawnTimer(SpawnTimer&& c) = delete;
	SpawnTimer& operator=(SpawnTimer&& rhs) = delete;

	void Update(float elapsedSec, int wave);
	int GetSpawnWave() const;
	bool CanSpawn(int wave) const;
	Enemy* GetEnemy();

private:

	const int m_Wave;
	float m_SpawnDelay;
	Enemy* m_pEnemy;
};

