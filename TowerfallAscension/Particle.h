#pragma once
#include "utils.h"
class Particle final
{
public:
	Particle(const Point2f& position, float size, const Vector2f& speedVector, 
		float lifeTime, const Color4f& color = Color4f{ 1.f, 1.f, 1.f, 1.f }, bool changeAlpha = true);
	~Particle() = default;
	Particle(const Particle& c) = delete;
	Particle& operator=(const Particle& rhs) = delete;
	Particle(Particle&& c) = delete;
	Particle& operator=(Particle&& rhs) = delete;


	virtual void Update(float elapsedSec);
	virtual void Draw() const;

	bool ShouldDelete();
	void UpdateMoveVector(const Vector2f& moveVector);

private:

	bool m_ShouldDelete;
	bool m_ChangeAlpha;
	float m_Size;
	float m_LifeCounter;
	const float m_LifeTime;
	Color4f m_Color;
	const Vector2f m_MoveVector;
	Vector2f m_SpeedVector;
	Point2f m_Position;
};

