#include "pch.h"
#include "UpgradeManager.h"
#include "Chest.h"
#include "Upgrade.h"
#include <iostream>
#include "LevelTemplate.h"
#include "Level.h"
#include "Player.h"
#include "Keyboard.h"
#include "Resourcemanager.h"
#include "SoundManager.h"

UpgradeManager::UpgradeManager(const std::vector<ChestInfo>& chests, ResourceManager* resources)
	:m_CurrentAmount{}
	,m_pResources{resources}
{
	for (size_t i{}; i < chests.size(); ++i)
	{
		m_pChests.push_back(new Chest(chests[i].chestSpawn, chests[i].upgradeType, resources));
	}
}

UpgradeManager::~UpgradeManager()
{
	for (size_t i{}; i < m_pUpgrades.size(); ++i)
	{
		delete m_pUpgrades[i];
	}
	m_pUpgrades.clear();

	for (size_t i{}; i < m_pChests.size(); ++i)
	{
		delete m_pChests[i];
	}
	m_pChests.clear();
}

void UpgradeManager::Update(float elapsedSec, const Level& level)
{
	for (size_t i{}; i < m_pUpgrades.size(); ++i)
	{
		m_pUpgrades[i]->Update(elapsedSec, level);
	}
	for (int i{}; i < m_CurrentAmount; ++i)
	{
		m_pChests[i]->Update(elapsedSec, level);
	}
}

void UpgradeManager::Draw() const
{

	for (size_t i{}; i < m_pUpgrades.size(); ++i)
	{
		m_pUpgrades[i]->Draw();
	}
	for (int i{}; i < m_CurrentAmount; ++i)
	{
		m_pChests[i]->Draw();
	}
}

void UpgradeManager::SpawnChest()
{
	if (m_CurrentAmount < int(m_pChests.size()))
	{
		m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::CHEST_APPEAR);
		++m_CurrentAmount;
	}
}

void UpgradeManager::AttemptOpenChest(const Rectf& actorShape)
{
	for (int i{}; i < m_CurrentAmount; ++i)
	{
		Upgrade* pUpgrade = m_pChests[i]->OpenChest(actorShape);
		if (pUpgrade)
		{
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::CHEST_OPEN);
			m_pUpgrades.push_back(pUpgrade);
		}
	}
}

void UpgradeManager::AttemptPickupUpgrade(Player& player, const Keyboard& keyboard)
{
	for (size_t i{}; i < m_pUpgrades.size(); ++i)
	{
		if (m_pUpgrades[i]->CheckHit(player.GetActorShape(), keyboard) && player.SetUpgrade(m_pUpgrades[i]->GetUpgrade()))
		{			
			delete m_pUpgrades[i];
			m_pUpgrades[i] = m_pUpgrades.back();
			m_pUpgrades.pop_back();
			
		}
	}
}

void UpgradeManager::AddUpgrade(Upgrade* upgrade)
{
	m_pUpgrades.push_back(upgrade);
}