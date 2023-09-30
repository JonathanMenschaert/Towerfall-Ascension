#include "pch.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Arrow.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "Matrix2x3.h"
#include "Level.h"

Arrow::Arrow(const Point2f& position, float angle, ResourceManager* resources, float force)
	:Enemy(false, resources)
	, m_Trajectory{ cosf(angle * float(M_PI / 180.f)) * force, sinf(angle * float(M_PI / 180.f)) * force }
	, m_TransformedVertices{}
	, m_MaxFallSpeed{ -500.f }
	, m_IsPickup{false}
{
	if (angle > 90.f || angle < -90.f)
	{
		m_Angle = 180.f - angle;
	}
	else
	{
		m_Angle = angle;
	}
	Vector2f scale{ m_Scale, m_Scale };
	m_pArrow = new Sprite(resources->GetTextureManager()->GetTexture(TextureManager::TextureSheet::ARROW_PLAYER));
	m_pArrow->SetScale(scale);

	m_Translation = Vector2f{ position };

	m_Shape = Rectf{ 0.f, 0.f, m_pArrow->GetWidth(), m_pArrow->GetHeight() };

	m_Vertices.push_back(Point2f{ 0.f, 0.f });
	m_Vertices.push_back(Point2f{ m_Shape.width, 0.f });
	m_Vertices.push_back(Point2f{ m_Shape.width, m_Shape.height});
	m_Vertices.push_back(Point2f{ 0.f, m_Shape.height });
	m_Location = Point2f{ m_Shape.width / 2.f, m_Shape.height / 2.f };
}

Arrow::~Arrow()
{
	delete m_pArrow;
	m_pArrow = nullptr;
}

void Arrow::Update(float elapsedSec, const Level& level)
{
	if (!m_IsPickup)
	{
		m_Translation += m_Trajectory * elapsedSec;
		if (m_Trajectory.y > m_MaxFallSpeed)
		{
			m_Trajectory += m_Acceleration * elapsedSec;
		}

		m_Angle = atan2(m_Trajectory.y, m_Trajectory.x) * float(180.f / M_PI);
		if (m_Angle > 90.f || m_Angle < -90.f)
		{
			m_Angle = 180.f - m_Angle;
			if (m_Angle >= 360.f)
			{
				m_Angle -= 360.f;
			}
			else if (m_Angle <= -360.f)
			{
				m_Angle += 360.f;
			}
		}

		Matrix2x3 matTranslate, matRotate, matScale, matSource;
		matTranslate.SetAsTranslate(m_Translation);
		matRotate.SetAsRotate(m_Angle);
		matScale.SetAsScale(m_Trajectory.x > 0 ? m_Scale : -m_Scale, m_Scale);
		matSource.SetAsTranslate(-m_pArrow->GetWidth() / 2, -m_pArrow->GetHeight() / 2);

		Matrix2x3 matWorld{ matTranslate * matScale * matRotate * matSource };
		m_TransformedVertices = matWorld.Transform(m_Vertices);
		m_TransformedLocation = matWorld.Transform(m_Location);

		HandleLevelBoundaries();
		m_IsPickup = level.HandleCollision(m_TransformedVertices);
	}
}

void Arrow::HandleLevelBoundaries()
{

	Point2f bottomLeft{ m_LevelBoundaries.left, m_LevelBoundaries.bottom };
	Point2f bottomRight{ m_LevelBoundaries.left + m_LevelBoundaries.width, m_LevelBoundaries.bottom };
	Point2f TopRight{ m_LevelBoundaries.left + m_LevelBoundaries.width, m_LevelBoundaries.bottom + m_LevelBoundaries.height};
	Point2f TopLeft{ m_LevelBoundaries.left, m_LevelBoundaries.bottom + m_LevelBoundaries.height };

	utils::HitInfo hitInfo{};

	if (utils::Raycast(m_TransformedVertices, bottomLeft, TopLeft, hitInfo))
	{
		if (m_Translation.x < m_LevelBoundaries.left)
		{
			m_Translation.x += m_LevelBoundaries.width;
			m_CopySprite = CopyDirection::LEFT;
		}
		else
		{
			m_CopySprite = CopyDirection::RIGHT;
		}
	}
	else if (utils::Raycast(m_TransformedVertices, bottomRight, TopRight, hitInfo))
	{
		if (m_Translation.x > m_LevelBoundaries.left + m_LevelBoundaries.width)
		{
			m_Translation.x -= m_LevelBoundaries.width;
			m_CopySprite = CopyDirection::RIGHT;
		}
		else
		{
			m_CopySprite = CopyDirection::LEFT;
		}
	}
	else if (utils::Raycast(m_TransformedVertices, bottomLeft, bottomRight, hitInfo))
	{
		if (m_Translation.y < m_LevelBoundaries.bottom)
		{
			m_Translation.y += m_LevelBoundaries.height;
			m_CopySprite = CopyDirection::DOWN;
		}
		else
		{
			m_CopySprite = CopyDirection::UP;
		}
	}
	else if (utils::Raycast(m_TransformedVertices, TopLeft, TopRight, hitInfo))
	{
		if (m_Translation.y > m_LevelBoundaries.bottom + m_LevelBoundaries.height)
		{
			m_Translation.y -= m_LevelBoundaries.height;
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

void Arrow::Draw() const
{
	CopyDirection copyState{ m_CopySprite };

	int amount{};
	if (copyState == CopyDirection::NONE)
	{
		amount = 1;
	}
	else
	{
		amount = 2;
	}
	for (int i{}; i < amount; ++i)
	{
		glPushMatrix();
		{
			switch (copyState)
			{
			case CopyDirection::DOWN:
				glTranslatef(0.f, -m_LevelBoundaries.height, 0.f);
				break;
			case CopyDirection::UP:
				glTranslatef(0.f, m_LevelBoundaries.height, 0.f);
				break;
			case CopyDirection::LEFT:
				glTranslatef(-m_LevelBoundaries.width, 0.f, 0.f);
				break;
			case CopyDirection::RIGHT:
				glTranslatef(m_LevelBoundaries.width, 0.f, 0.f);
				break;
			default:
				break;
			}
			glTranslatef(m_Translation.x, m_Translation.y, 0.f);
			glScalef(m_Trajectory.x > 0 ? 1.f : -1.f, 1.f, 1.f);
			glRotatef(m_Angle, 0.f, 0.f, 1.f);
			glTranslatef(-m_pArrow->GetWidth() / 2 * m_Scale, -m_pArrow->GetHeight() / 2 * m_Scale, 0.f);
			m_pArrow->Draw();
		}
		glPopMatrix();
		copyState = CopyDirection::NONE;
	}
}

bool Arrow::IsPickUp() const
{
	return m_IsPickup;
}

const std::vector<Point2f>& Arrow::GetTransformedShape() const
{
	return m_TransformedVertices;
}

void Arrow::UpdateTrajectory(float elapsedSec, float gravity, const Vector2f& distance)
{
	Vector2f distanceNorm{ distance.Normalized() };
	m_Trajectory.x += distanceNorm.x * gravity * elapsedSec;
	m_Trajectory.y += distanceNorm.y * gravity * elapsedSec;
}

Point2f Arrow::GetPosition() const
{
	return m_TransformedLocation;
}
