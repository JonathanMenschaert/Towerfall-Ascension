#pragma once
#include "GameObject.h"
#include "Upgrade.h"

class AnimatedSprite;
class ResourceManager;
class Chest final : public GameObject
{
public:

	Chest(const Point2f& location, ResourceManager* resources);
	Chest(const Point2f& location, Upgrade::UpgradeType upgrade, ResourceManager* resources);
	virtual ~Chest();
	Chest(const Chest& c) = delete;
	Chest& operator=(const Chest& rhs) = delete;
	Chest(Chest&& c) = delete;
	Chest& operator=(Chest&& rhs) = delete;

	virtual void Update(float elapsedSec, const Level& level) override;
	virtual void Draw() const override;

	Upgrade* OpenChest(const Rectf& actorShape);

private:

	AnimatedSprite* m_pChestTexture;
	Upgrade* m_pUpgrade;
	bool m_ReleaseUpgrade;
	bool m_IsUpgradeActive;
};

