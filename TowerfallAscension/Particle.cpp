#include "pch.h"
#include "Particle.h"

Particle::Particle(const Point2f& position, float size, const Vector2f& speedVector, float lifeTime, const Color4f& color, bool changeAlpha)
	:m_Size{ size }
	,m_MoveVector{}
	,m_SpeedVector{speedVector}
	,m_LifeTime{lifeTime}
	,m_Color{color}
	,m_ChangeAlpha{changeAlpha}
	,m_Position{position}
	,m_LifeCounter{}
	,m_ShouldDelete{false}
{
}

void Particle::Update(float elapsedSec)
{
	
	m_Position += m_SpeedVector * elapsedSec;
	m_LifeCounter += elapsedSec;
	if (m_LifeCounter >= m_LifeTime)
	{
		m_ShouldDelete = true;
	}
}

void Particle::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_Position.x - m_Size / 2.f, m_Position.y - m_Size / 2.f, 0.f);
		utils::SetColor(m_Color);
		utils::FillRect(m_Position.x, m_Position.y, m_Size, m_Size);
	}
	glPopMatrix();
}

bool Particle::ShouldDelete()
{
	return m_ShouldDelete;
}

void Particle::UpdateMoveVector(const Vector2f& moveVector)
{
}
