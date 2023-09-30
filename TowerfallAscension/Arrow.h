#pragma once
#include "Enemy.h"
#include <vector>

class Sprite;
class ResourceManager;
class Arrow final : public Enemy
{
public:
	Arrow(const Point2f& position, float angle, ResourceManager* resources, float force = 1000.f);
	virtual ~Arrow();

	Arrow(const Arrow& c) = delete;
	Arrow& operator=(const Arrow& rhs) = delete;
	Arrow(Arrow&& c) = delete;
	Arrow& operator=(Arrow&& rhs) = delete;

	virtual void Update(float elapsedSec, const Level& level) override;
	virtual void Draw() const override;
	virtual void HandleLevelBoundaries() override;
	virtual Point2f GetPosition() const override;

	bool IsPickUp() const;
	const std::vector<Point2f>& GetTransformedShape() const;
	void UpdateTrajectory(float elapsedSec, float gravity, const Vector2f& distance);

private:

	bool m_IsPickup;
	float m_Angle;
	const float m_MaxFallSpeed;
	Vector2f m_Translation;
	Vector2f m_Trajectory;
	Sprite* m_pArrow;

	std::vector<Point2f> m_Vertices, m_TransformedVertices;
	Point2f m_Location, m_TransformedLocation;
};

