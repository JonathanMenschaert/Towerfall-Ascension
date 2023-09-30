#include "pch.h"
#include "Hat.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "Level.h"
#include "Keyboard.h"

Hat::Hat(const Point2f& position, ResourceManager* resources)
	:Upgrade(UpgradeType::HAT, position, resources)
{
	Texture* pHat{ m_pResources->GetTextureManager()->GetTexture(TextureManager::TextureSheet::UPGRADE_HAT) };
	m_pHat = new Sprite(pHat);
	m_pHat->SetScale(Vector2f{ m_Scale, m_Scale });

	m_Shape = Rectf{ position.x, position.y, m_pHat->GetWidth() * m_Scale, m_pHat->GetHeight() * m_Scale};
}

Hat::~Hat()
{
	delete m_pHat;
	m_pHat = nullptr;
}

void Hat::Update(float elapsedSec, const Level& level)
{
	m_Velocity += m_Acceleration * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
	HandleLevelBoundaries();
	level.HandleCollision(m_Shape, m_Velocity);
	
}
void Hat::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_Shape.left, m_Shape.bottom, 0.f);
		m_pHat->Draw();
	}
	glPopMatrix();
}

bool Hat::CheckHit(const Rectf& actorShape, const Keyboard& keyboard)
{
	return utils::IsOverlapping(actorShape, m_Shape) && keyboard.IsCrouchPressed();
}

