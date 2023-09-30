#pragma once
#include "GameObject.h"

class AnimatedSprite;
class ResourceManager;
class Enemy : public GameObject
{
public:
	enum class EnemyType
	{
		SLIME,
		GHOST
	};

	Enemy(const Point2f& location, float followDistance, ResourceManager* resources);
	Enemy(bool isEnemyActive, ResourceManager* resources);
	Enemy(const Point2f& location, bool isEnemyActive, float followDistance, ResourceManager* resources);
	virtual ~Enemy();

	Enemy(const Enemy& c) = delete;
	Enemy& operator=(const Enemy& rhs) = delete;
	Enemy(Enemy&& c) = delete;
	Enemy& operator=(Enemy&& rhs) = delete;

	virtual void Update(float elapsedSec, const Level& level) override;
	virtual void Draw() const override;

	bool IsDead() const;
	bool ShouldDelete() const;

	float GetGravityForce() const;
	float GetGravityRange() const;

	float GetReactionDistance() const;

	bool SetSpawnerActive();
	virtual bool AttemptDodge(const Level& level, const Point2f& actorLocation, const Vector2f& actorVelocity);

	static void SetPlayerPosition(const Point2f& playerPos);
	static void SetPlayerDead(bool isPlayerDead);

protected:

	bool m_IsDead;
	bool m_IsSetDelete;
	bool m_IsSpawnerActive;

	const Point2f m_SpawnLocation;

	static Point2f m_PlayerPosition;
	static bool m_IsPlayerDead;

	virtual bool CheckPlayerInRange(const Level& level, const Point2f& p1);

private:	
	
	AnimatedSprite* m_pSpawner;	
	float m_SpawnerTime;
	const float m_ViewDistance;
	const float m_MaxSpawnerTime;
	const float m_GravityForce;
	const float m_GravityRange;
};

