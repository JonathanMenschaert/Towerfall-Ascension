#include "pch.h"
#include "AnimatedSprite.h"
#include "Texture.h"

AnimatedSprite::AnimatedSprite(const Texture* pSprite, const Rectf& clipRect, 
	int rows, int cols, int framesPerSec, int nrOfFrames, AnimationState animType)
	: Sprite(pSprite, clipRect)
	, m_Rows{rows}
	, m_Cols{cols}
	, m_NrOfFrames{nrOfFrames}
	, m_FramesPerSec{framesPerSec}
	, m_CurrentFrame{}
	, m_AnimTime{}
	, m_AnimState{animType}
{

}

void AnimatedSprite::Draw(const Vector2f& offset) const
{
	glPushMatrix();
	{
		glTranslatef(offset.x * m_Scale.x, offset.y * m_Scale.y, 0.f);
		glScalef(m_Scale.x, m_Scale.y, 1.f);
		m_pSprite->Draw(Point2f{}, Rectf{
			m_ClipRect.left + (m_CurrentFrame % m_Cols) * m_ClipRect.width,
			m_ClipRect.bottom + (m_CurrentFrame / m_Cols) * m_ClipRect.height,
			m_ClipRect.width,
			m_ClipRect.height
			});
	}
	glPopMatrix();
}

void AnimatedSprite::Update(float elapsedSec)
{
	switch (m_AnimState)
	{
	case AnimationState::LOOP_END:
		m_AnimTime += elapsedSec;
		if (m_AnimTime > 1.f / m_FramesPerSec)
		{
			m_AnimTime -= 1.f / m_FramesPerSec;
			if (m_CurrentFrame == m_NrOfFrames - 1)
			{
				m_AnimState = AnimationState::FREEZE;
			}
			else
			{
				++m_CurrentFrame;
			}
		}
		break;
	case AnimationState::LOOP_ONCE:
		m_AnimTime += elapsedSec;
		if (m_AnimTime > 1.f / m_FramesPerSec)
		{
			m_AnimTime -= 1.f / m_FramesPerSec;
			if ((++m_CurrentFrame %= m_NrOfFrames) == 0)
			{
				m_AnimState = AnimationState::FREEZE;
			}
		}
		break;
	case AnimationState::LOOP:
		m_AnimTime += elapsedSec;
		if (m_AnimTime > 1.f / m_FramesPerSec)
		{
			m_AnimTime -= 1.f / m_FramesPerSec;
			++m_CurrentFrame %= m_NrOfFrames;
		}
		break;
	}

	
}

void AnimatedSprite::StartAnimation()
{
	m_AnimState = AnimationState::LOOP_ONCE;
}

void AnimatedSprite::EndAnimation()
{
	m_AnimState = AnimationState::FREEZE;
}

void AnimatedSprite::NextFrame(int startFrame)
{
	if (startFrame < 0)
	{
		++m_CurrentFrame; 
	}
	else
	{
		m_CurrentFrame = startFrame;
	}
	m_CurrentFrame %= m_NrOfFrames;
}

int AnimatedSprite::GetCurrentFrame() const
{
	return m_CurrentFrame;
}

int AnimatedSprite::GetTotalFrames() const
{
	return m_NrOfFrames;
}
