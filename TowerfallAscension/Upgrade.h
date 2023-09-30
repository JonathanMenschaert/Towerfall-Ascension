#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "AnimatedSprite.h"

class Keyboard;
class ResourceManager;
class Upgrade : public GameObject
{
public:

	enum class UpgradeType
	{
		SHIELD,
		ARROWS,

		//add chest upgrades above
		AMOUNT,
		HAT,
		NONE
	};

	Upgrade(UpgradeType upgrade, const Point2f& position, ResourceManager* resources);
	virtual ~Upgrade();
	Upgrade(const Upgrade& c) = delete;
	Upgrade& operator=(const Upgrade& rhs) = delete;
	Upgrade(Upgrade&& c) = delete;
	Upgrade& operator=(Upgrade&& rhs) = delete;

	virtual void Update(float elapsedSec, const Level& level) override;
	virtual void Draw() const override;
	virtual bool AttemptKill() override;

	virtual bool CheckHit(const Rectf& actorShape, const Keyboard& keyboard);
	
	UpgradeType GetUpgrade() const;

protected:
	UpgradeType m_UpgradeType;

private:
	float m_JitterRadius;
	Point2f m_JitterCenter;
	Vector2f m_Jitter;
	const float m_VertSpeed;
	
	Sprite* m_pUpgradeForeground;
	AnimatedSprite* m_pUpgradeBackground;

	Vector2f CreateJitterVector();
};

