#include "pch.h"
#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite(const Texture* pSprite, const Rectf& clipRect)
	: m_pSprite{ pSprite }
	, m_ClipRect{clipRect}
	, m_Scale{ 1.f, 1.f }
	, m_Angle{ 0.f }
{
}

Sprite::Sprite(const Texture* pSprite)
	: Sprite(pSprite, Rectf{0.f, pSprite->GetHeight(), pSprite->GetWidth(), pSprite->GetHeight()})
{
}


void Sprite::Draw(const Vector2f& offset) const
{
	glPushMatrix();
	{
		glTranslatef(offset.x * m_Scale.x, offset.y * m_Scale.y, 0.f);
		glScalef(m_Scale.x, m_Scale.y, 1.f);
		m_pSprite->Draw(Point2f{}, m_ClipRect);
	}
	glPopMatrix();
}

void Sprite::Update(float elapsedSec)
{
}

void Sprite::SetScale(const Vector2f& scale)
{
	m_Scale = scale;
}

void Sprite::SetAngle(float angle)
{
	m_Angle = angle;
}

float Sprite::GetHeight() const
{
	return m_ClipRect.height;
}

float Sprite::GetWidth() const
{
	return m_ClipRect.width;
}

void Sprite::SetClipPosition(const Point2f& position)
{
	m_ClipRect.left = position.x;
	m_ClipRect.bottom = position.y;
}

float Sprite::GetScaledWidth() const
{
	return m_ClipRect.width* m_Scale.x;
}

float Sprite::GetScaledHeight() const
{
	return m_ClipRect.height * m_Scale.y;
}
