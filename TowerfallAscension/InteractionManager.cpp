#include "pch.h"
#include "InteractionManager.h"
#include "Enemy.h"
#include "Arrow.h"
#include "Hat.h"
#include <iostream>
#include "Player.h"
#include "Level.h"
#include "UpgradeManager.h"

InteractionManager::InteractionManager(const Rectf& boundaries, ResourceManager* resources)
	:m_pEnemies{}
	,m_pArrows{}
	,m_LevelBoundaries{boundaries}
	,m_pResources{resources}
{
}

InteractionManager::~InteractionManager()
{
	for (unsigned int i{}; i < m_pEnemies.size(); ++i)
	{
		delete m_pEnemies[i];
	}
	m_pEnemies.clear();

	for (unsigned int i{}; i < m_pArrows.size(); ++i)
	{
		delete m_pArrows[i];
	}
	m_pArrows.clear();
}

void InteractionManager::Update(float elapsedSec, const Level& level, Player& player, UpgradeManager& upgradeManager)
{
	UpdateEnemies(elapsedSec, level, player, upgradeManager);
	UpdateArrows(elapsedSec, level, player);
	
}

void InteractionManager::UpdateEnemies(float elapsedSec, const Level& level, Player& player, UpgradeManager& upgradeManager)
{
	Rectf actorShape{ player.GetActorShape() };
	Rectf bottomHalf{ actorShape.left, actorShape.bottom, actorShape.width, actorShape.height / 2.f };
	Rectf topHalf{ actorShape.left, actorShape.bottom + actorShape.height / 2.f, actorShape.width, actorShape.height / 2.f };

	Enemy::SetPlayerPosition(Point2f{ actorShape.left + actorShape.width / 2.f, actorShape.bottom + actorShape.height / 2 });
	Enemy::SetPlayerDead(player.IsDead());

	for (size_t i{}; i < m_pEnemies.size(); ++i)
	{
		if (!m_pEnemies[i]->IsDead())
		{
			m_pEnemies[i]->Update(elapsedSec, level);

			if (utils::IsOverlapping(m_pEnemies[i]->GetActorShape(), topHalf))
			{
				AttemptPlayerKill(int(i), player, upgradeManager);
			}
			else if (!player.IsDead() && utils::IsOverlapping(m_pEnemies[i]->GetActorShape(), bottomHalf)
				&& player.GetVelocity().y < 0.f)
			{
				if (m_pEnemies[i]->AttemptKill())
				{
					float jumpForce{ 400.f };
					player.GetVelocity().y = jumpForce;
				}
			}
			else if (player.IsDead() && player.HasExploded())
			{
				Rectf enemyShape{ m_pEnemies[i]->GetActorShape() };
				Point2f enemyLoc{ enemyShape.left + enemyShape.width / 2.f, enemyShape.bottom + enemyShape.height / 2.f };
				Point2f playerLoc{ actorShape.left + actorShape.width / 2.f, actorShape.bottom + actorShape.height / 2.f };

				Vector2f range{ enemyLoc - playerLoc };
				float distanceSq{ range.SquaredLength() };
				if (std::powf(player.GetExplosionRadius(), 2.f) > distanceSq)
				{
					m_pEnemies[i]->AttemptKill();
				}
			}
		}
		else if (!m_pEnemies[i]->ShouldDelete())
		{
			m_pEnemies[i]->Update(elapsedSec, level);
		}
		else
		{
			delete m_pEnemies[i];
			m_pEnemies[i] = m_pEnemies.back();
			m_pEnemies.pop_back();
		}
	}
}

void InteractionManager::AttemptPlayerKill(unsigned int enemyIdx, Player& player, UpgradeManager& upgradeManager)
{
	if (player.AttemptKill())
	{
		if (player.IsDead() && player.HasHat())
		{
			upgradeManager.AddUpgrade(new Hat(Point2f{ player.GetActorShape().left, player.GetActorShape().bottom }, m_pResources));
		}
		else
		{
			float pushForce{ 300.f };
			player.GetVelocity().x = (player.GetActorShape().left > m_pEnemies[enemyIdx]->GetActorShape().left ? pushForce : -pushForce);
		}
	}
	
}

void InteractionManager::UpdateArrows(float elapsedSec, const Level& level, Player& player)
{
	for (size_t i{}; i < m_pArrows.size(); ++i)
	{
		m_pArrows[i]->Update(elapsedSec, level);
		for (size_t j{}; j < m_pEnemies.size(); ++j)
		{
			if (!m_pArrows[i]->IsPickUp())
			{
				Vector2f distance{ m_pEnemies[j]->GetPosition() - m_pArrows[i]->GetPosition() };
				float gravityRange{ m_pEnemies[j]->GetGravityRange() };
				if (distance.SquaredLength() <= std::powf(gravityRange, 2.f))
				{
					m_pArrows[i]->UpdateTrajectory(elapsedSec, m_pEnemies[j]->GetGravityForce(), distance);
				}
				if (IsOverlapping(m_pArrows[i]->GetTransformedShape(), m_pEnemies[j]->GetActorShape()))
				{
					m_pEnemies[j]->AttemptKill();
				}
				else if (distance.SquaredLength() < std::powf(m_pEnemies[j]->GetReactionDistance(), 2.f))
				{
					m_pEnemies[j]->AttemptDodge(level, m_pArrows[i]->GetPosition(), m_pArrows[i]->GetVelocity());
				}
			}
		}
		if (m_pArrows[i]->IsPickUp() && IsOverlapping(m_pArrows[i]->GetTransformedShape(), player.GetActorShape()))
		{
			player.AddArrow();
			delete m_pArrows[i];
			m_pArrows[i] = m_pArrows.back();
			m_pArrows.pop_back();
		}
	}
}

bool InteractionManager::IsOverlapping(const std::vector<Point2f>& actorShape1, const Rectf& actorShape2) const
{
	Point2f bottomLeft{ actorShape2.left, actorShape2.bottom };
	Point2f bottomRight{ actorShape2.left + actorShape2.width, actorShape2.bottom };
	Point2f TopRight{ actorShape2.left + actorShape2.width, actorShape2.bottom + actorShape2.height };
	Point2f TopLeft{ actorShape2.left, actorShape2.bottom + actorShape2.height };

	utils::HitInfo hitInfo{};

	return utils::Raycast(actorShape1, bottomLeft, TopLeft, hitInfo)
		|| utils::Raycast(actorShape1, bottomRight, TopRight, hitInfo)
		|| utils::Raycast(actorShape1, bottomLeft, bottomRight, hitInfo)
		|| utils::Raycast(actorShape1, TopLeft, TopRight, hitInfo);
}

void InteractionManager::Draw() const
{

	for (unsigned int i{}; i < m_pArrows.size(); ++i)
	{
		m_pArrows[i]->Draw();
	}
	for (unsigned int i{}; i < m_pEnemies.size(); ++i)
	{
		m_pEnemies[i]->Draw();
	}
}

void InteractionManager::ShootArrow(Arrow* arrow)
{
	m_pArrows.push_back(arrow);
	m_pArrows.back()->SetLevelBoundaries(m_LevelBoundaries);
}

void InteractionManager::AddEnemy(Enemy* enemy)
{
	m_pEnemies.push_back(enemy);
	m_pEnemies.back()->SetLevelBoundaries(m_LevelBoundaries);
}

int InteractionManager::GetEnemiesLeft() const
{
	return int(m_pEnemies.size());
}


