#include "pch.h"
#include "Chest.h"
#include "Texture.h"
#include "AnimatedSprite.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "Hat.h"
#include "Level.h"
#include <iostream>

Chest::Chest(const Point2f& location, ResourceManager* resources)
	:Chest(location, Upgrade::UpgradeType(rand() % int(Upgrade::UpgradeType::AMOUNT)), resources)
{
}

Chest::Chest(const Point2f& location, Upgrade::UpgradeType upgrade, ResourceManager* resources)
	:GameObject(resources)
	,m_ReleaseUpgrade{false}
	,m_IsUpgradeActive{false}
	//,m_pUpgrade{new Upgrade(Upgrade::UpgradeType::SHIELD, location)}
{
	Vector2f scale{ m_Scale, m_Scale };
	Texture* pChest{ resources->GetTextureManager()->GetTexture(TextureManager::TextureSheet::UPGRADE_CHEST) };
	m_pChestTexture = new AnimatedSprite{ pChest, Rectf{0.f, pChest->GetHeight(), pChest->GetWidth() / 4, pChest->GetHeight()},
		1, 4, 8, 4, AnimatedSprite::AnimationState::LOOP_END };
	m_pChestTexture->SetScale(scale);

	m_Shape = Rectf{ location.x - scale.x * pChest->GetWidth() / 8.f, location.y,
		scale.x * pChest->GetWidth() / 4.f, scale.y * pChest->GetHeight() };

	m_pUpgrade = new Upgrade(upgrade, location, resources);
}

Chest::~Chest()
{
	delete m_pChestTexture;
	m_pChestTexture = nullptr;

	delete m_pUpgrade;
	m_pUpgrade = nullptr;
}

void Chest::Update(float elapsedSec, const Level& level)
{
	if (m_ReleaseUpgrade)
	{
		m_pChestTexture->Update(elapsedSec);
		if (m_pChestTexture->GetCurrentFrame() == m_pChestTexture->GetTotalFrames() -1)
		{
			m_ReleaseUpgrade = false;
		}
	}
	m_Velocity += m_Acceleration * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
	level.HandleCollision(m_Shape, m_Velocity);
}

void Chest::Draw() const
{

	glPushMatrix();
	{
		glTranslatef(m_Shape.left, m_Shape.bottom, 0.f);
		m_pChestTexture->Draw();
	}
	glPopMatrix();
	//utils::DrawRect(m_Shape);
}

Upgrade* Chest::OpenChest(const Rectf& actorShape)
{
	if (utils::IsOverlapping(m_Shape, actorShape) && !( m_IsUpgradeActive || m_ReleaseUpgrade))
	{
		m_ReleaseUpgrade = true;
		m_IsUpgradeActive = true;
		Upgrade* pUpgrade = m_pUpgrade;
		m_pUpgrade = nullptr;
		return pUpgrade;
	}
	return nullptr;
}