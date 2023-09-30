#include "pch.h"
#include "Ghost.h"
#include "Texture.h"
#include "AnimatedSprite.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include <iostream>
#include "Level.h"
#include "SoundManager.h"

Ghost::Ghost(const Point2f& location, ResourceManager* resources)
	:Enemy(location, 200.f, resources)
	,m_GhostState{GhostState::SPAWN}
	, m_Destination{}
	, m_HorSpeed{110.f}
	, m_VertSpeed{110.f}
	, m_DestinationRadius{5.f}
	, m_CurrentPathPosition{-1}
	, m_DistToDestination{FLT_MAX}
	, m_IdleTime{}
	, m_MaxIdleTime{1.f}
	, m_IdleChance{10}
	, m_ConfusionTime{0.f}
	, m_MaxConfusionTime{5.f}
	, m_IsConfused{false}
	, m_DodgeVector{}
{
	m_Scale = 3.f;
	InitGhost();
}

void Ghost::InitGhost()
{
	Vector2f scale{ m_Scale, m_Scale };
	TextureManager* textureManager{ m_pResources->GetTextureManager() };
	Texture* pWander{ textureManager->GetTexture(TextureManager::TextureSheet::GHOST_WANDERING) };
	Texture* pDodge{ textureManager->GetTexture(TextureManager::TextureSheet::GHOST_DODGE) };
	Texture* pDeath{ textureManager->GetTexture(TextureManager::TextureSheet::GHOST_DYING) };
	Texture* pConfusion{textureManager->GetTexture(TextureManager::TextureSheet::GHOST_CONFUSION) };

	m_pWander = new AnimatedSprite(pWander, Rectf{ 0.f, pWander->GetHeight(), pWander->GetWidth() / 5, pWander->GetHeight() },
		1, 5, 5, 5, AnimatedSprite::AnimationState::LOOP);

	m_pDodge = new AnimatedSprite(pDodge, Rectf{ 0.f, pDodge->GetHeight() / 4, pDodge->GetWidth() / 4, pDodge->GetHeight() / 4 },
		4, 4, 40, 16, AnimatedSprite::AnimationState::LOOP);

	m_pDeath = new AnimatedSprite(pDeath, Rectf{ 0.f, pDeath->GetHeight() / 3, pDeath->GetWidth() / 4, pDeath->GetHeight() / 3 },
		3, 4, 12, 12, AnimatedSprite::AnimationState::LOOP);

	m_pConfusion = new AnimatedSprite(pConfusion, Rectf{ 0.f, pConfusion->GetHeight(), pConfusion->GetWidth() / 9, pConfusion->GetHeight() },
		1, 9, 12, 9, AnimatedSprite::AnimationState::LOOP);

	m_pWander->SetScale(scale);
	m_pDodge->SetScale(scale);
	m_pDeath->SetScale(scale);
	m_pConfusion->SetScale(scale);

	m_Shape = Rectf{ m_SpawnLocation.x, m_SpawnLocation.y, pWander->GetWidth() / 5 * scale.x, pWander->GetHeight() * scale.y };

	m_Destination = m_SpawnLocation;
}

Ghost::~Ghost()
{
	delete m_pWander;
	m_pWander = nullptr;

	delete m_pDodge;
	m_pDodge = nullptr;

	delete m_pDeath;
	m_pDeath = nullptr;

	delete m_pConfusion;
	m_pConfusion = nullptr;
}

void Ghost::Update(float elapsedSec, const Level& level)
{
	switch (m_GhostState)
	{
	case GhostState::SPAWN:
		Enemy::Update(elapsedSec, level);
		if (!m_IsDead)
		{
			if (!m_IsSpawnerActive)
			{
				m_GhostState = GhostState::WANDER;
			}
		}
		break;
	case GhostState::WANDER:
	{
		m_pWander->Update(elapsedSec);
		Point2f actorLocation{ m_Shape.left + m_Shape.width / 2.f, m_Shape.bottom + m_Shape.height / 2.f };
		Vector2f moveVector{ };

		if (utils::IsPointInCircle(actorLocation, Circlef{ m_Destination, m_DestinationRadius }))
		{
			if (rand() % m_IdleChance == 0)
			{
				m_GhostState = GhostState::IDLE;
			}
			else
			{
				GetNextDestinationPoint(actorLocation, level);
			}
		}
		
		moveVector = Vector2f{ m_Destination.x - actorLocation.x, m_Destination.y - actorLocation.y };
		float distanceSq{ moveVector.SquaredLength() };
		if (distanceSq <= m_DistToDestination)
		{
			m_DistToDestination = distanceSq;
		}
		else
		{
			GetNextDestinationPoint(actorLocation, level);
		}
		
		if (CheckPlayerInRange(level, m_PlayerPosition))
		{
			m_GhostState = GhostState::HUNT;
		}
		UpdatePosition(elapsedSec, moveVector.Normalized());
	}
	break;
	case GhostState::HUNT:
	{		
		m_pWander->Update(elapsedSec);
		Point2f actorLocation{ m_Shape.left + m_Shape.width / 2.f, m_Shape.bottom + m_Shape.height / 2.f };
		m_Destination = m_PlayerPosition;
		Vector2f moveVector{ m_Destination.x - actorLocation.x, m_Destination.y - actorLocation.y };
		if (!CheckPlayerInRange(level, m_PlayerPosition))
		{
			m_GhostState = GhostState::WANDER;			
		}
		UpdatePosition(elapsedSec, moveVector.Normalized());
	}
	break;
	case GhostState::IDLE:
		m_pWander->Update(elapsedSec);
		m_IdleTime += elapsedSec;
		if (m_IdleTime >= m_MaxIdleTime)
		{
			m_Velocity.x = rand() % 2 == 0 ? m_Velocity.x : -m_Velocity.x;
			m_Velocity.y = rand() % 2 == 0 ? m_Velocity.y : -m_Velocity.y;
			m_GhostState = GhostState::WANDER;
			m_IdleTime = 0.f;
		}
		else if (CheckPlayerInRange(level, m_PlayerPosition))
		{
			m_GhostState = GhostState::HUNT;
			m_IdleTime = 0.f;
		}
		break;
	case GhostState::DODGE:
	{
		Point2f actorLocation{ GetPosition()};
		m_pDodge->Update(elapsedSec);
		UpdatePosition(elapsedSec, m_DodgeVector * 2.f);
		level.HandleCollision(m_Shape, m_Velocity);
		if (m_pDodge->GetCurrentFrame() == m_pDodge->GetTotalFrames() - 1)
		{
			m_GhostState = GhostState::WANDER;
			m_DistToDestination = FLT_MAX;
		}
	}
		break;
	case GhostState::DEATH:
		m_pDeath->Update(elapsedSec);
 		if (m_pDeath->GetCurrentFrame() == m_pDeath->GetTotalFrames() - 1)
		{
			m_IsSetDelete = true;
		}
		break;
	}
	if (m_IsConfused)
	{
		m_pConfusion->Update(elapsedSec);
		m_ConfusionTime += elapsedSec;
		if (m_ConfusionTime >= m_MaxConfusionTime)
		{
			m_IsConfused = false;
		}
	}

	HandleLevelBoundaries();
}

void Ghost::Draw() const
{
	if (m_IsSpawnerActive)
	{
		Enemy::Draw();
	}

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
			glTranslatef(m_Shape.left + m_Shape.width, m_Shape.bottom, 0.f);
			glScalef(-1.f, 1.f, 1.f);
		}
		else
		{
			glTranslatef(m_Shape.left, m_Shape.bottom, 0.f);
		}

		switch (m_GhostState)
		{
		case GhostState::SPAWN:
			if (!m_IsDead)
			{
				m_pWander->Draw();
			}
			break;
		case GhostState::HUNT:
		case GhostState::IDLE:
		case GhostState::WANDER:
			m_pWander->Draw();
			break;
		case GhostState::DODGE:
			m_pDodge->Draw();
			break;
		case GhostState::DEATH:
			m_pDeath->Draw();
			break;
		}

		if (m_IsConfused && m_GhostState != GhostState::DEATH)
		{
			glTranslatef(m_Shape.width / 2.f - m_pConfusion->GetScaledWidth() / 2.f, m_Shape.height, 0.f);
			m_pConfusion->Draw();
		}

		glPopMatrix();
		copyState = CopyDirection::NONE;
	}	
}

void Ghost::UpdatePosition(float elapsedSec, const Vector2f& moveVectorNorm)
{
	m_Velocity.x = moveVectorNorm.x * m_HorSpeed;
	m_Velocity.y = moveVectorNorm.y * m_VertSpeed;
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
}

void Ghost::GetNextDestinationPoint(const Point2f& currentLocation, const Level& level)
{
	m_Destination = level.HandleFloatingPath(currentLocation, m_Velocity, m_CurrentPathPosition);
	m_DistToDestination = FLT_MAX;
}

bool Ghost::AttemptKill()
{
	if (!m_IsDead && m_GhostState != GhostState::DEATH && m_GhostState != GhostState::DODGE)
	{
		m_GhostState = GhostState::DEATH;
		m_pDeath->NextFrame(0);
		m_IsDead = true;
		m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::GHOST_DEATH);
		return true;
	}
	return false;
}

bool Ghost::AttemptDodge(const Level& level, const Point2f& actorLocation, const Vector2f& actorVelocity)
{
	if (!m_IsConfused && m_GhostState != GhostState::DODGE && m_GhostState != GhostState::DEATH)
	{
		Point2f actorPosition{ m_Shape.left + m_Shape.width / 2.f, m_Shape.bottom + m_Shape.height / 2.f };
		if ((actorLocation.x > actorPosition.x && m_Velocity.x >= 0.f && actorVelocity.x <= 0.f) || 
			(actorLocation.x < actorPosition.x && m_Velocity.x < 0.f && actorVelocity.x >= 0.f))
		{
			Vector2f dodgeVector{ actorPosition - actorLocation };
			m_DodgeVector = (dodgeVector / 2.f).Normalized();
			m_GhostState = GhostState::DODGE;
			m_IsConfused = true;
			m_ConfusionTime = 0.f;
			m_pDodge->NextFrame(0);
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::GHOST_DODGE);
			return true;
		}		
	}
	return false;
}

bool Ghost::CheckPlayerInRange(const Level& level, const Point2f& p1)
{
	return !m_IsPlayerDead && Enemy::CheckPlayerInRange(level, p1);
}

