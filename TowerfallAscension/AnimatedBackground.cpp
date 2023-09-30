#include "pch.h"
#include "AnimatedBackground.h"
#include "Sprite.h"
#include "Texture.h"

AnimatedBackground::AnimatedBackground(const Texture* pSprite, const Vector2f& movingVector)
	:Sprite(pSprite)
	, m_MovingVector{ movingVector }
	, m_TopLeft{0.f, pSprite->GetHeight()}
{
}

void AnimatedBackground::Draw(const Vector2f& offset) const
{
	Sprite::Draw(offset);
}

void AnimatedBackground::Update(float elapsedSec)
{
	m_ClipRect.left += m_MovingVector.x * elapsedSec;
	if (m_ClipRect.left > m_ClipRect.width)
	{
		m_ClipRect.left -= m_ClipRect.width;
	}

	m_ClipRect.bottom += m_MovingVector.y * elapsedSec;
	if (m_ClipRect.bottom > 2 * m_ClipRect.height)
	{
		m_ClipRect.left -= m_ClipRect.height;
	}
}


