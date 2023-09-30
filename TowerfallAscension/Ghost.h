#pragma once
#include "Enemy.h"
#include <vector>

//Work in progress
class AnimatedSprite;
class ResourceManager;
class Ghost final : public Enemy
{
public:
	enum class GhostState
	{
		SPAWN,
		WANDER,
		DODGE,
		DEATH,
		IDLE,
		HUNT
	};

	explicit Ghost(const Point2f& location, ResourceManager* resources);
	virtual ~Ghost();
	Ghost(const Ghost& c) = delete;
	Ghost& operator=(const Ghost& rhs) = delete;
	Ghost(Ghost&& c) = delete;
	Ghost& operator=(Ghost&& rhs) = delete;

	virtual void Update(float elapsedSec, const Level& level) override;
	virtual void Draw() const override;
	virtual bool AttemptKill() override;
	virtual bool AttemptDodge(const Level& level, const Point2f& actorLocation, const Vector2f& actorVelocity) override;
	virtual bool CheckPlayerInRange(const Level& level, const Point2f& p1) override;

	void InitGhost();

private:
	
	bool m_IsConfused;

	int m_IdleChance;
	int m_CurrentPathPosition;

	float m_DistToDestination;
	float m_IdleTime;
	float m_ConfusionTime;

	const float m_HorSpeed;
	const float m_VertSpeed;
	const float m_DestinationRadius;
	const float m_MaxIdleTime;	
	const float m_MaxConfusionTime;

	AnimatedSprite* m_pWander;
	AnimatedSprite* m_pDodge;
	AnimatedSprite* m_pDeath;
	AnimatedSprite* m_pConfusion;

	GhostState m_GhostState;
	Point2f m_Destination;
	Vector2f m_DodgeVector;

	void UpdatePosition(float elapsedSec, const Vector2f& moveVectorNorm);
	void GetNextDestinationPoint(const Point2f& currentLocation, const Level& level);
};

