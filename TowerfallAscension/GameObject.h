#pragma once
#include "utils.h"

class Level;
class ResourceManager;
class GameObject
{
public:
	enum class CopyDirection
	{
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	GameObject(ResourceManager* resources, float scale = 3.5f);
	virtual ~GameObject() = default;
	GameObject(const GameObject& c) = delete;
	GameObject& operator=(const GameObject& rhs) = delete;
	GameObject(GameObject&& c) = delete;
	GameObject& operator=(GameObject&& rhs) = delete;

	virtual void Update(float elapsedSec, const Level& level) = 0;
	virtual void Draw() const = 0;

	void SetLevelBoundaries(const Rectf& boundaries);
	virtual void HandleLevelBoundaries();

	const Rectf& GetActorShape() const;
	virtual Point2f GetPosition() const;
	virtual bool AttemptKill();
	

	virtual Vector2f& GetVelocity();

protected:
	float m_Scale;

	Vector2f m_Velocity;
	const Vector2f m_Acceleration;
	
	Rectf m_LevelBoundaries;
	Rectf m_Shape;
	
	CopyDirection m_CopySprite;

	ResourceManager* m_pResources;
};

