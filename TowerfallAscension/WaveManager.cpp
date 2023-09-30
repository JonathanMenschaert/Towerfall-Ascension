#include "pch.h"
#include "WaveManager.h"
#include "SpawnTimer.h"
#include <iostream>
#include "LevelTemplate.h"
#include "Slime.h"
#include "Ghost.h"
#include "InteractionManager.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "UpgradeManager.h"
#include "Level.h"
#include "Texture.h"

WaveManager::WaveManager(const Window& window, const std::vector<EnemyInfo>& enemies, ResourceManager* resources)
	:m_Wave{1}
	,m_MaxWaves{FindHighestWave(enemies)}
	,m_WaveState{WaveState::TRANSITION_MOVEIN}
	,m_Window{window}
	,m_VerticalPos{}
	,m_WaveScale{1.f}
	,m_IdleTime{}
	,m_MaxIdleTime{1.f}
	,m_MaxVerticalSpeed{450.f}
	,m_MaxWaveScale{1.5f}
	,m_ScaleSpeed{0.75f}
	,m_Acceleration{210.f}
	,m_ReachedEnd{false}
{
	m_VerticalSpeed = m_MaxVerticalSpeed;

	TextureManager* textureManager{ resources->GetTextureManager() };
	m_pShield = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::UPGRADE_SHIELD));
	
	m_pShield->SetScale(Vector2f{2.5f, 2.5f});

	Vector2f scale{ 4.f, 4.f };
	Texture* pWaveTexture{ textureManager->GetTexture(TextureManager::TextureSheet::WAVE_NUMBER) };
	
	for (size_t i{}; i < size_t(m_MaxWaves); ++i)
	{
		m_pWaveNumbers.push_back(new Sprite(pWaveTexture,
			Rectf{ pWaveTexture->GetWidth(), (i + 1) * pWaveTexture->GetHeight() * 0.1f,
			pWaveTexture->GetWidth(), pWaveTexture->GetHeight() * 0.1f }));
		m_pWaveNumbers[i]->SetScale(scale);
	}	
	for (size_t i{}; i < enemies.size(); ++i)
	{
		m_pWaveList.push_back(new SpawnTimer(CreateEnemy(enemies[i], resources), enemies[i].wave, enemies[i].spawnDelay));
	}
}

WaveManager::~WaveManager()
{
	for (size_t i{}; i < m_pWaveList.size(); ++i)
	{
		delete m_pWaveList[i];
	}
	m_pWaveList.clear();

	delete m_pShield;
	m_pShield = nullptr;
	
	for (size_t i{}; i < m_pWaveNumbers.size(); ++i)
	{
		delete m_pWaveNumbers[i];
	}
	m_pWaveNumbers.clear();
}

void WaveManager::Update(float elapsedSec, InteractionManager& interactionManager, UpgradeManager& upgradeManager, const Level& level)
{
	switch (m_WaveState)
	{
	case WaveState::TRANSITION_MOVEIN:
		//Animate wave number
		if (m_VerticalPos >= m_Window.height * 0.55f)
		{
			m_WaveState = WaveState::TRANSITION_SCALEUP;
			m_VerticalSpeed = 0.f;
		}
		else
		{
			m_VerticalSpeed -= m_Acceleration * elapsedSec;
			m_VerticalPos += m_VerticalSpeed * elapsedSec;
		}
		break;
	case WaveState::TRANSITION_SCALEUP:
		//Animate wave number
		if (m_WaveScale >= m_MaxWaveScale)
		{
			m_WaveState = WaveState::TRANSITION_IDLE;
			m_IdleTime = 0.f;
		}
		else
		{
			m_WaveScale += m_ScaleSpeed * elapsedSec;
		}
		break;
	case WaveState::TRANSITION_IDLE:
		//Animate wave number
		if (m_IdleTime >= m_MaxIdleTime)
		{
			m_WaveState = WaveState::TRANSITION_SCALEDOWN;
			m_IdleTime = 0.f;
		}
		else
		{
			m_IdleTime += elapsedSec;
		}
		break;
	case WaveState::TRANSITION_SCALEDOWN:
		//Animate wave number
		if (m_WaveScale <= 1.f)
		{
			m_WaveState = WaveState::TRANSITION_MOVEOUT;
			m_IdleTime = 0.f;
			m_WaveScale = 1.f;
			m_VerticalSpeed = m_MaxVerticalSpeed;
		}
		else
		{
			m_WaveScale -= m_ScaleSpeed * elapsedSec;
		}
		break;
	case WaveState::TRANSITION_MOVEOUT:
		//Animate wave number
		if (m_VerticalPos >= m_Window.height * 1.05f)
		{
			m_VerticalSpeed = m_MaxVerticalSpeed;
			m_VerticalPos = 0.f;
			m_WaveState = WaveState::START;
		}
		else
		{
			m_VerticalSpeed += m_Acceleration * elapsedSec;
			m_VerticalPos += m_VerticalSpeed * elapsedSec;
		}
		break;
	case WaveState::START:
	{
		bool waveEnded{ true };
		for (size_t i{}; i < m_pWaveList.size(); ++i)
		{
			if (m_pWaveList[i]->GetSpawnWave() == m_Wave)
			{
				waveEnded = false;
			}

			m_pWaveList[i]->Update(elapsedSec, m_Wave);
			if (m_pWaveList[i]->CanSpawn(m_Wave))
			{
				interactionManager.AddEnemy(m_pWaveList[i]->GetEnemy());
				delete m_pWaveList[i];
				m_pWaveList[i] = m_pWaveList.back();
				m_pWaveList.pop_back();
			}
		}

		if (waveEnded && interactionManager.GetEnemiesLeft() == 0)
		{
			if (m_Wave < m_MaxWaves)
			{
				//std::cout << "Next wave" << std::endl;
				upgradeManager.SpawnChest();
				++m_Wave;
				m_WaveState = WaveState::TRANSITION_MOVEIN;
			}
			else
			{
				m_WaveState = WaveState::END;
			}
		}
	}
	break;
	case WaveState::END:
		m_ReachedEnd = true;
		break;
	default:
		break;
	}
}

void WaveManager::AddSpawn(SpawnTimer* spawnTimer)
{
	m_pWaveList.push_back(spawnTimer);
}

Enemy* WaveManager::CreateEnemy(const EnemyInfo& enemy, ResourceManager* resources)
{
	switch (enemy.enemyType)
	{
	case Enemy::EnemyType::SLIME:
		return new Slime(enemy.spawnPoint, resources);
	case Enemy::EnemyType::GHOST:
		return new Ghost(enemy.spawnPoint, resources);
	default:
		return nullptr;
	}
}

void WaveManager::Draw() const
{
	if (m_WaveState != WaveState::START && m_WaveState != WaveState::END)
	{
		glPushMatrix();
		{
			glTranslatef(m_Window.width * 0.5f - (m_pWaveNumbers[0]->GetScaledWidth() * m_pWaveNumbers.size()) / 2.f,
				m_VerticalPos - m_pWaveNumbers[0]->GetScaledHeight() * 0.5f, 0.f);
			for (size_t i{}; i < m_pWaveNumbers.size(); ++i)
			{
				if (m_Wave == int(i + 1) && (m_WaveState == WaveState::TRANSITION_SCALEUP ||
					m_WaveState == WaveState::TRANSITION_SCALEDOWN || m_WaveState == WaveState::TRANSITION_IDLE))
				{
					DrawShield(m_pWaveNumbers[i]);
				}
				else
				{
					m_pWaveNumbers[i]->Draw();
				}
				glTranslatef(m_pWaveNumbers[0]->GetScaledWidth(), 0.f, 0.f);
			}
		}
		glPopMatrix();
	}
}

bool WaveManager::HasReachedEnd() const
{
	return m_ReachedEnd;
}

void WaveManager::DrawShield(const Sprite* number) const
{
	
		
		glPushMatrix();
		{			
			glTranslatef(-(m_WaveScale - 1.f) * (m_pShield->GetScaledWidth() + number->GetScaledWidth()) * 0.25f - 
				m_pShield->GetWidth() * 0.25f, 0.f, 0.f);
			glScalef(m_WaveScale, m_WaveScale, 0.f);
			glTranslatef(0.f, -m_pShield->GetScaledHeight() * 0.25f, 0.f);
			m_pShield->Draw();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(-number->GetScaledWidth() * (m_WaveScale - 1.f) * 0.5f, 0.f, 0.f);
			glScalef(m_WaveScale, m_WaveScale, 0.f);
			
			number->Draw();
		}
		glPopMatrix();

}

int WaveManager::FindHighestWave(const std::vector<EnemyInfo>& enemies)
{
	int highest{};
	for (size_t i{}; i < enemies.size(); ++i)
	{
		if (enemies[i].wave > highest)
		{
			highest = enemies[i].wave;
		}
	}
	return highest;
}
