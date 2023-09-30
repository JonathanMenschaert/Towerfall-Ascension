#pragma once
#include "Enemy.h"
class ResourceManager;
class Slime final : public Enemy
{
public:
	enum class SlimeState
	{
		SPAWN,
		WANDER,
		FALL,
		LAND,
		PREY,
		DEATH
	};

	Slime(const Point2f& location, ResourceManager* resources);
	virtual ~Slime();
	Slime(const Slime& c) = delete;
	Slime& operator=(const Slime& rhs) = delete;
	Slime(Slime&& c) = delete;
	Slime& operator=(Slime&& rhs) = delete;

	virtual void Update(float elapsedSec, const Level& level) override;
	virtual void Draw() const override;
	virtual bool AttemptKill() override;

	void InitSlime();

protected:
	virtual bool CheckPlayerInRange(const Level& level, const Point2f& p1) override;

private:
	const float m_HorSpeed;
	const float m_JumpSpeed;
	const float m_MaxFallSpeed;
	const float m_MaxDistanceTravelled;

	float m_HorDistanceTravelled;

	Vector2f m_Velocity;
	SlimeState m_SlimeState;
	AnimatedSprite* m_pWander;
	AnimatedSprite* m_pFalling;
	AnimatedSprite* m_pLanding;
	AnimatedSprite* m_pDying;
};

