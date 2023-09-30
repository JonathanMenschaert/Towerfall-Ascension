#include "pch.h"
#include "Enemy.h"
#include "Texture.h"
#include "AnimatedSprite.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include <iostream>
#include "Level.h"
#include "SoundManager.h"


Point2f Enemy::m_PlayerPosition{ 0.f, 0.f };
bool Enemy::m_IsPlayerDead{ true };

Enemy::Enemy(const Point2f& location, float followDistance, ResourceManager* resources)
	:Enemy(location, true, followDistance, resources)
{
}

Enemy::Enemy(bool isDead, ResourceManager* resources)
	: Enemy(Point2f{}, isDead, 0.f, resources)
{
}


Enemy::Enemy(const Point2f& location, bool isDead, float followDistance, ResourceManager* resources)
	:GameObject(resources)
	, m_SpawnLocation{ location }
	, m_IsSpawnerActive{ false }
	, m_IsDead{ isDead }
	, m_SpawnerTime{}
	, m_MaxSpawnerTime{ 5.f }
	,m_ViewDistance{followDistance}
	,m_IsSetDelete{false}
	,m_GravityForce{3000.f}
	,m_GravityRange{100.f}
{
	Vector2f scale{ m_Scale, m_Scale };

	Texture* pPortal{ resources->GetTextureManager()->GetTexture(TextureManager::TextureSheet::SPAWN_PORTAL) };
	m_pSpawner = new AnimatedSprite(pPortal, Rectf{ 0.f, pPortal->GetHeight() / 5, pPortal->GetWidth() / 6, pPortal->GetHeight() / 5 },
		5, 6, 7, 28, AnimatedSprite::AnimationState::LOOP);

	m_pSpawner->SetScale(scale);
}

Enemy::~Enemy()
{
	delete m_pSpawner;
	m_pSpawner = nullptr;
}

void Enemy::Update(float elapsedSec, const Level& level)
{
	if (m_IsSpawnerActive)
	{
		m_pSpawner->Update(elapsedSec);
		m_SpawnerTime += elapsedSec;
		if (m_SpawnerTime >= m_MaxSpawnerTime)
		{
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PORTAL_DISAPPEAR);
			m_IsSpawnerActive = false;
		}
		else if (m_IsDead && m_SpawnerTime >= m_MaxSpawnerTime * 0.75f)
		{
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PORTAL_SPAWN);
			m_IsDead = false;
		}
	}
}


void Enemy::Draw() const
{
	if (m_IsSpawnerActive)
	{
		glPushMatrix();
		{
			glTranslatef(m_SpawnLocation.x, m_SpawnLocation.y, 0.f);
			m_pSpawner->Draw();
		}
		glPopMatrix();
	}
}

bool Enemy::AttemptDodge(const Level& level, const Point2f& actorLocation, const Vector2f& actorVelocity)
{
	return false;
}

bool Enemy::IsDead() const
{
	return m_IsDead;
}

bool Enemy::ShouldDelete() const
{
	return m_IsSetDelete;
}

float Enemy::GetGravityForce() const
{
	return m_GravityForce;
}

float Enemy::GetGravityRange() const
{
	return m_GravityRange;
}

float Enemy::GetReactionDistance() const
{
	return m_ViewDistance / 2.f;
}

void Enemy::SetPlayerPosition(const Point2f& playerPos)
{
	m_PlayerPosition = playerPos;
}

void Enemy::SetPlayerDead(bool isPlayerDead)
{
	m_IsPlayerDead = isPlayerDead;
}


bool Enemy::SetSpawnerActive()
{
	m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PORTAL_APPEAR);
	return m_IsSpawnerActive = true;
}

bool Enemy::CheckPlayerInRange(const Level& level, const Point2f& p1)
{
	Point2f p2{ m_Shape.left + m_Shape.width / 2.f, m_Shape.bottom + m_Shape.height / 2.f };

	float followSq{ std::powf(m_ViewDistance, 2.f) };
	float distanceSq{ std::powf(p1.x - p2.x, 2.f) + std::powf(p1.y - p2.y, 2.f) };
	if ( distanceSq <= followSq)
	{
		return !level.CheckHit(p1, p2);
	}
	return false;
}