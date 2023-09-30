#pragma once
#include <vector>

class Enemy;
class SpawnTimer;
struct EnemyInfo;
class InteractionManager;
class UpgradeManager;
class Level;
class ResourceManager;
class Sprite;
class WaveManager final
{
public:

	enum class WaveState
	{
		INACTIVE,
		TRANSITION_MOVEIN,
		TRANSITION_SCALEUP,
		TRANSITION_IDLE,
		TRANSITION_SCALEDOWN,
		TRANSITION_MOVEOUT,
		START,		
		END
	};
	explicit WaveManager(const Window& window, const std::vector<EnemyInfo>& enemies, ResourceManager* resources);
	~WaveManager();

	WaveManager(const WaveManager& c) = delete;
	WaveManager& operator=(const WaveManager& rhs) = delete;
	WaveManager(WaveManager&& c) = delete;
	WaveManager& operator=(WaveManager&& rhs) = delete;

	void Update(float elapsedSec, InteractionManager& interactionManager, UpgradeManager& upgradeManager, const Level& level);
	void AddSpawn(SpawnTimer* spawnTimer);
	void Draw() const;

	bool HasReachedEnd() const;

private:

	bool m_ReachedEnd;

	int m_Wave;
	const int m_MaxWaves;

	float m_IdleTime;
	const float m_MaxIdleTime;

	float m_VerticalPos;
	float m_VerticalSpeed;
	const float m_MaxVerticalSpeed;
	const float m_Acceleration;

	float m_WaveScale;
	const float m_ScaleSpeed;
	const float m_MaxWaveScale;

	const Window m_Window;
	
	WaveState m_WaveState;
	Sprite* m_pShield;
	std::vector<Sprite*> m_pWaveNumbers;
	std::vector<SpawnTimer*> m_pWaveList;

	int FindHighestWave(const std::vector<EnemyInfo>& enemies);
	Enemy* CreateEnemy(const EnemyInfo& enemy, ResourceManager* resources);
	void DrawShield(const Sprite* number) const;
};

