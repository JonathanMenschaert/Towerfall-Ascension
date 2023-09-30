#include "pch.h"
#include "Slime.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "AnimatedSprite.h"
#include <iostream>
#include "Level.h"
#include "SoundManager.h"


Slime::Slime(const Point2f& location, ResourceManager* resources)
	:Enemy(location, 150.f, resources)
	, m_HorSpeed{ 100.f }
	, m_JumpSpeed{ 500.f }
	, m_SlimeState{SlimeState::SPAWN}
	, m_HorDistanceTravelled{}
	, m_MaxFallSpeed{ -400.f }
	, m_MaxDistanceTravelled{ 20.f }
{
	m_Velocity = Vector2f{ m_HorSpeed, 0.f };
	InitSlime();
}

void Slime::InitSlime()
{
	Vector2f scale{ m_Scale, m_Scale };
	Texture* pSlime{ m_pResources->GetTextureManager()->GetTexture(TextureManager::TextureSheet::SLIME_MOVEMENT) };
	m_pWander = new AnimatedSprite(pSlime, Rectf{ 0.f, pSlime->GetHeight() / 2.f, pSlime->GetWidth() / 7.f, pSlime->GetHeight() / 4.f},
		1, 6, 6, 6, AnimatedSprite::AnimationState::LOOP);
	m_pWander->SetScale(scale);

	m_pFalling = new AnimatedSprite(pSlime, Rectf{ 0.f, pSlime->GetHeight() * 0.75f, pSlime->GetWidth() / 7.f, pSlime->GetHeight() / 4.f },
		1, 4, 6, 4, AnimatedSprite::AnimationState::LOOP);
	m_pFalling->SetScale(scale);

	m_pLanding = new AnimatedSprite(pSlime, Rectf{ 0.f, pSlime->GetHeight() / 4.f, pSlime->GetWidth() / 7.f, pSlime->GetHeight() / 4.f },
		1, 7, 14, 7, AnimatedSprite::AnimationState::LOOP);
	m_pLanding->SetScale(scale);

	m_pDying = new AnimatedSprite(pSlime, Rectf{ 0.f, pSlime->GetHeight(), pSlime->GetWidth() / 7.f, pSlime->GetHeight() / 4.f },
		1, 4, 8, 4, AnimatedSprite::AnimationState::LOOP);
	m_pDying->SetScale(scale);

	const float width{ pSlime->GetWidth() / 7.f * scale.x };
	const float height{ pSlime->GetHeight() / 4.f * scale.y };
	m_Shape = Rectf{ m_SpawnLocation.x + width / 4.f, m_SpawnLocation.y, width / 2.f, height / 2.f};
}

Slime::~Slime()
{
	delete m_pWander;
	m_pWander = nullptr;

	delete m_pFalling;
	m_pFalling = nullptr;

	delete m_pLanding;
	m_pLanding = nullptr;

	delete m_pDying;
	m_pDying = nullptr;
}

void Slime::Update(float elapsedSec, const Level& level)
{
	switch (m_SlimeState)
	{
	case SlimeState::SPAWN:
		Enemy::Update(elapsedSec, level);
		if (!m_IsDead)
		{
			//m_pWander->Update(elapsedSec);
			if (!m_IsSpawnerActive)
			{
				m_SlimeState = SlimeState::WANDER;
			}
		}
		break;
	case SlimeState::PREY:
		if (!CheckPlayerInRange(level, m_PlayerPosition))
		{
			m_SlimeState = SlimeState::WANDER;
		}
	case SlimeState::WANDER:
		{
			m_pWander->Update(elapsedSec);

			m_Velocity += m_Acceleration * elapsedSec;
			if (m_Velocity.y <= m_MaxFallSpeed)
			{
				m_Velocity.y = m_MaxFallSpeed;
			}
			m_Shape.left += m_Velocity.x * elapsedSec;
			m_Shape.bottom += m_Velocity.y * elapsedSec;
			level.HandleCollision(m_Shape, m_Velocity);

			bool isColliding{ level.IsTouchingWall(m_Shape, m_Velocity) && level.IsOnGround(m_Shape, m_Velocity) };
			if (isColliding && m_HorDistanceTravelled >= m_MaxDistanceTravelled)
			{
				m_HorDistanceTravelled = 0.f;
				m_Velocity.x = (m_Velocity.x > 0.f ? -m_HorSpeed : m_HorSpeed);
			}
			else
			{
				m_HorDistanceTravelled += m_HorSpeed * elapsedSec;
			}
			if (CheckPlayerInRange(level, m_PlayerPosition))
			{
				m_SlimeState = SlimeState::PREY;
			}
			if (m_Velocity.y < m_MaxFallSpeed * 0.8f)
			{
				m_SlimeState = SlimeState::FALL;
			}
		}		
		break;
	case SlimeState::FALL:

		m_pFalling->Update(elapsedSec);
		m_Velocity += m_Acceleration * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		level.HandleCollision(m_Shape, m_Velocity);

		if (m_Velocity.y <= m_MaxFallSpeed)
		{
			m_Velocity.y = m_MaxFallSpeed;
		}
		else if (m_Velocity.y == 0.f)
		{
			m_pLanding->NextFrame(0);
			m_SlimeState = SlimeState::LAND;
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::SLIME_LAND);
		}
		break;
	case SlimeState::LAND:
		m_pLanding->Update(elapsedSec);
		level.HandleCollision(m_Shape, m_Velocity);
		if (m_pLanding->GetCurrentFrame() == m_pLanding->GetTotalFrames() - 1)
		{
			m_SlimeState = SlimeState::WANDER;
		}
		break;
	case SlimeState::DEATH:
		m_pDying->Update(elapsedSec);
		if (m_pDying->GetCurrentFrame() == m_pDying->GetTotalFrames() - 1)
		{
			m_IsSetDelete = true;
		}
		break;
	}
	HandleLevelBoundaries();
}

void Slime::Draw() const
{
	if (m_IsSpawnerActive)
	{
		Enemy::Draw();
	}

	CopyDirection copyState{ m_CopySprite };
	//utils::DrawRect(m_Shape);
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

		if (m_Velocity.x < 0)
		{
			glTranslatef(m_Shape.left + m_Shape.width * 1.5f, m_Shape.bottom, 0.f);
			glScalef(-1.f, 1.f, 1.f);
		}
		else
		{
			glTranslatef(m_Shape.left - m_Shape.width / 2.f, m_Shape.bottom, 0.f);
		}

		switch (m_SlimeState)
		{
		case SlimeState::SPAWN:
			if (!m_IsDead)
			{
				m_pWander->Draw();
			}
			break;
		case SlimeState::PREY:
		case SlimeState::WANDER:
			m_pWander->Draw();
			break;
		case SlimeState::FALL:
			m_pFalling->Draw();
			break;
		case SlimeState::LAND:
			m_pLanding->Draw();
			break;
		case SlimeState::DEATH:
			m_pDying->Draw();
			break;
		}
		glPopMatrix();
		copyState = CopyDirection::NONE;
	}
}

bool Slime::AttemptKill()
{
	if (!m_IsDead && m_SlimeState != SlimeState::DEATH)
	{
		m_SlimeState = SlimeState::DEATH;
		m_pDying->NextFrame(0);
		m_IsDead = true;
		m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::SLIME_DEATH);
		return true;
	}
	return false;
}

bool Slime::CheckPlayerInRange(const Level& level, const Point2f& p1)
{
	if (Enemy::CheckPlayerInRange(level, p1))
	{
		if (!m_IsPlayerDead && (m_Shape.left + m_Shape.width < p1.x || m_Shape.left > p1.x) 
			&& (m_Shape.bottom + 1.2 * m_Shape.height >= p1.y))
		{
			bool isMoreRight{ m_Shape.left > p1.x };
			m_Velocity.x = (isMoreRight ? -m_HorSpeed : m_HorSpeed);
			return true;
		}		
	}	
	return false;
}