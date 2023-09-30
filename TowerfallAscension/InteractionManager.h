#pragma once
#include <vector>

class Enemy;
class Arrow;
class Level;
class Player;
class UpgradeManager;
class ResourceManager;

class InteractionManager final
{
public:
	InteractionManager(const Rectf& boundaries, ResourceManager* resources);
	~InteractionManager();

	InteractionManager(const InteractionManager& c) = delete;
	InteractionManager& operator=(const InteractionManager& rhs) = delete;
	InteractionManager(InteractionManager&& c) = delete;
	InteractionManager& operator=(InteractionManager&& rhs) = delete;

	void Update(float elapsedSec, const Level& level, Player& player, UpgradeManager& upgradeManager);
	void UpdateEnemies(float elapsedSec, const Level& level, Player& player, UpgradeManager& upgradeManager);
	void UpdateArrows(float elapsedSec, const Level& level, Player& player);
	void AttemptPlayerKill(unsigned int enemyIdx, Player& player, UpgradeManager& upgradeManager);
	void Draw() const;

	void ShootArrow(Arrow* arrow);
	void AddEnemy(Enemy* enemy);
	int GetEnemiesLeft() const;

private:

	std::vector<Enemy*> m_pEnemies;
	std::vector<Arrow*> m_pArrows;
	Rectf m_LevelBoundaries;
	ResourceManager* m_pResources;

	bool IsOverlapping(const std::vector<Point2f>& actorShape1, const Rectf& actorShape2) const;
};

