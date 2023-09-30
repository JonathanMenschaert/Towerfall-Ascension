#include "pch.h"
#include "GameObject.h"
#include "Level.h"

GameObject::GameObject(ResourceManager* resources, float scale)
	:m_CopySprite{ CopyDirection::NONE }
	,m_Scale{scale}
	,m_Acceleration{0.f, -1500.f}
	,m_Velocity{0.f, 0.f}
	,m_pResources{resources}
{
}

void GameObject::SetLevelBoundaries(const Rectf& boundaries)
{
	m_LevelBoundaries = boundaries;
}

void GameObject::HandleLevelBoundaries()
{
	if (m_Shape.left < m_LevelBoundaries.left)
	{
		if (m_Shape.left + m_Shape.width / 2 < m_LevelBoundaries.left)
		{
			m_Shape.left += m_LevelBoundaries.width;
			m_CopySprite = CopyDirection::LEFT;
		}
		else
		{
			m_CopySprite = CopyDirection::RIGHT;
		}
	}
	else if (m_Shape.left + m_Shape.width > m_LevelBoundaries.left + m_LevelBoundaries.width)
	{
		if (m_Shape.left + m_Shape.width / 2 > m_LevelBoundaries.left + m_LevelBoundaries.width)
		{
			m_Shape.left -= m_LevelBoundaries.width;
			m_CopySprite = CopyDirection::RIGHT;
		}
		else
		{
			m_CopySprite = CopyDirection::LEFT;
		}
	}
	else if (m_Shape.bottom < m_LevelBoundaries.bottom)
	{
		if (m_Shape.bottom + m_Shape.height / 2 < m_LevelBoundaries.bottom)
		{
			m_Shape.bottom += m_LevelBoundaries.height;
			m_CopySprite = CopyDirection::DOWN;
		}
		else
		{
			m_CopySprite = CopyDirection::UP;
		}
	}
	else if (m_Shape.bottom + m_Shape.height > m_LevelBoundaries.bottom + m_LevelBoundaries.height)
	{
		if (m_Shape.bottom + m_Shape.height / 2 > m_LevelBoundaries.bottom + m_LevelBoundaries.height)
		{
			m_Shape.bottom -= m_LevelBoundaries.height;
			m_CopySprite = CopyDirection::UP;
		}
		else
		{
			m_CopySprite = CopyDirection::DOWN;
		}
	}
	else
	{
		m_CopySprite = CopyDirection::NONE;
	}
}

const Rectf& GameObject::GetActorShape() const
{
	return m_Shape;
}

Point2f GameObject::GetPosition() const
{
	return Point2f { m_Shape.left + m_Shape.width / 2.f, m_Shape.bottom + m_Shape.height / 2.f };
}

Vector2f& GameObject::GetVelocity()
{
	return m_Velocity;
}

bool GameObject::AttemptKill()
{
	return false;
}
