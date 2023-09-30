#pragma once
#include <vector>

class Chest;
class Upgrade;
struct ChestInfo;
class Player;
class Level;
class Keyboard;
class ResourceManager;
class UpgradeManager final
{
public:
	UpgradeManager(const std::vector<ChestInfo>& chests, ResourceManager* resources);
	~UpgradeManager();

	UpgradeManager(const UpgradeManager& c) = delete;
	UpgradeManager& operator=(const UpgradeManager& rhs) = delete;
	UpgradeManager(UpgradeManager&& c) = delete;
	UpgradeManager& operator=(UpgradeManager&& rhs) = delete;

	void Update(float elapsedSec, const Level& level);
	void Draw() const;
	void SpawnChest();
	void AddUpgrade(Upgrade* upgrade);
	void AttemptOpenChest(const Rectf& actorShape);
	void AttemptPickupUpgrade(Player& player, const Keyboard& keyboard);

private:
	int m_CurrentAmount;
	std::vector<Chest*> m_pChests;
	std::vector<Upgrade*> m_pUpgrades;

	ResourceManager* m_pResources;
};

