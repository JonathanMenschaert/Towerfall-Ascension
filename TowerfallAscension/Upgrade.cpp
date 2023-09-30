#include "pch.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Upgrade.h"
#include "Texture.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Keyboard.h"

Upgrade::Upgrade(UpgradeType upgrade, const Point2f& position, ResourceManager* resources)
	:GameObject(resources)
	,m_UpgradeType{upgrade}
	,m_VertSpeed{150.f}
	,m_pUpgradeBackground{nullptr}
	,m_pUpgradeForeground{nullptr}
	,m_JitterRadius{6.f}
{
	Vector2f scale{ m_Scale, m_Scale };
	switch (upgrade)
	{
	case UpgradeType::SHIELD:
	{
		Texture* pAura{ resources->GetTextureManager()->GetTexture(TextureManager::TextureSheet::UPGRADE_SHIELD_AURA) };
		m_pUpgradeBackground = new AnimatedSprite(pAura, Rectf{ 0.f, pAura->GetHeight(), pAura->GetWidth() / 7.f, pAura->GetHeight() },
			1, 7, 8, 7, AnimatedSprite::AnimationState::LOOP);
		m_pUpgradeBackground->SetScale(scale);

		Texture* pShield{ resources->GetTextureManager()->GetTexture(TextureManager::TextureSheet::UPGRADE_SHIELD) };
		m_pUpgradeForeground = new Sprite(pShield);
		Vector2f scaleShield{ scale * 0.3f };
		m_pUpgradeForeground->SetScale(scaleShield);
		
		
		m_Shape = Rectf{ position.x - pShield->GetWidth() * scaleShield.x / 2.f, position.y, pShield->GetWidth() * scaleShield.x, pShield->GetHeight() * scaleShield.y };
	}
	break;
	case UpgradeType::ARROWS:
	{
		Texture* pArrow{ resources->GetTextureManager()->GetTexture(TextureManager::TextureSheet::UPGRADE_ARROW) };
		m_pUpgradeForeground = new Sprite(pArrow);
		m_pUpgradeForeground->SetScale(scale);


		m_Shape = Rectf{ position.x - pArrow->GetWidth() / 2.f, position.y, pArrow->GetWidth(), pArrow->GetHeight() };
		break;
	}
	default:
		break;
	}

	m_Velocity = Vector2f{ 0.f, m_VertSpeed };
	m_JitterCenter = Point2f{ m_Shape.left, m_Shape.bottom };
	float radians{ float((rand() % 360) * (M_PI / 180.f)) };
	m_Jitter = Vector2f{ m_JitterRadius * std::cosf(radians), m_JitterRadius * std::sinf(radians) };
}

Upgrade::~Upgrade()
{
	delete m_pUpgradeBackground;
	m_pUpgradeBackground = nullptr;

	delete m_pUpgradeForeground;
	m_pUpgradeForeground = nullptr;
}

bool Upgrade::CheckHit(const Rectf& actorShape, const Keyboard& keyboard)
{
	return utils::IsOverlapping(actorShape, m_Shape) && m_Velocity.y <= 0.f;
}

void Upgrade::Update(float elapsedSec, const Level& level)
{
	if (m_Velocity.y > 0.f)
	{
		m_Velocity += m_Acceleration * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
		m_JitterCenter = Point2f{m_Shape.left, m_Shape.bottom};
	}
	else
	{
		
		if (!utils::IsPointInCircle(Point2f{ m_Shape.left, m_Shape.bottom },
			Circlef{ m_JitterCenter, m_JitterRadius * 0.9f }))
		{
			m_Jitter = CreateJitterVector();
		}
		
		m_Shape.left += m_Jitter.x * elapsedSec;
		m_Shape.bottom += m_Jitter.y * elapsedSec;
		
	}
	switch (m_UpgradeType)
	{
	case UpgradeType::SHIELD:
		m_pUpgradeBackground->Update(elapsedSec);
		break;
	case UpgradeType::ARROWS:
		break;
	}
}

void Upgrade::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_Shape.left, m_Shape.bottom, 0.f);
		switch (m_UpgradeType)
		{
		case UpgradeType::SHIELD:
			glPushMatrix();
			{							
				glScalef( 0.75f, 0.75f, 1.f );
				glTranslatef(-m_pUpgradeBackground->GetWidth() / 2.f * m_Scale + m_Shape.width * 0.7f,
					-m_pUpgradeBackground->GetHeight() / 2.f * m_Scale + m_Shape.height * 0.7f, 0.f);
				m_pUpgradeBackground->Draw();
			}
			glPopMatrix();
			m_pUpgradeForeground->Draw();
			
			break;
		case UpgradeType::ARROWS:
			m_pUpgradeForeground->Draw();
			break;
		default:
			break;
		}
	}
	glPopMatrix();
	//utils::DrawRect(m_Shape);
}

bool Upgrade::AttemptKill()
{
	return false;
}

Upgrade::UpgradeType Upgrade::GetUpgrade() const
{
	return m_UpgradeType;
}

Vector2f Upgrade::CreateJitterVector()
{
	float offsetX{ float(rand() % int(m_JitterRadius + 1.f)) - m_JitterRadius / 2.f };
	float offsetY{ float(rand() % int(m_JitterRadius + 1.f)) - m_JitterRadius / 2.f };
	return Vector2f { m_JitterCenter.x - m_Shape.left + offsetX, m_JitterCenter.y - m_Shape.bottom + offsetY};
}