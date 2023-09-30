#include "pch.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Button.h"
#include "Texture.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "Text.h"

Button::Button(Texture* pTexture, const Point2f& location, const Point2f& startLocation, const std::string& buttonText, const Text::Size fontSize,
	Screen::ScreenType value, const ResourceManager* resources,const Vector2f& textOffset, float scale)
	:Widget(location, resources, scale, int(value))
	,m_pButtonTexture{new Sprite(pTexture)}
	,m_pButtonText{new Text(buttonText, int(fontSize), resources)}
	,m_ButtonState{ButtonState::DEACTIVATED}
	,m_ActivationSpeed{1500.f}
	,m_Rotation{0.f}
	,m_MaxRotation{360.f}
	,m_ScaleSpeed{4.f}
	,m_MaxTextScale{ scale * 0.75f }
	,m_MinTextScale{scale * 0.5f}
	,m_BobbingRadius{scale}
	,m_BobbingAngle{0.f}
	,m_BobbingSpeed{90.f}
	, m_TextOffset{ textOffset }
	,m_ActivationDistance{utils::GetDistance(location, startLocation)}
{
	SetMovePosition(startLocation);
	m_CurrentPosition = startLocation;
	m_TextScale = m_MinTextScale;
	m_MoveDirection = location - startLocation;
	m_MoveDirection = m_MoveDirection.Normalized() * m_ActivationSpeed;
}

Button::~Button()
{
	delete m_pButtonText;
	m_pButtonText = nullptr;

	delete m_pButtonTexture;
	m_pButtonTexture = nullptr;
}

void Button::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_CurrentPosition.x, m_CurrentPosition.y, 0.f);
		glPushMatrix();
		{		
			glRotatef(m_Rotation, 0.f, 0.f, 1.f);
			glScalef(m_Scale, m_Scale, 1.f);
			glTranslatef(-m_pButtonTexture->GetWidth() / 2.f, 
				-m_pButtonTexture->GetHeight() / 2.f + m_BobbingRadius * sinf(float(M_PI) / 180.f * m_BobbingAngle), 0.f);
			m_pButtonTexture->Draw();
		}
		glPopMatrix();
		
		glTranslatef(m_TextOffset.x - m_pButtonText->GetTextWidth() / 2.f * m_TextScale, 
			m_TextOffset.y - m_pButtonText->GetTextHeight() / 2.f * m_TextScale, 0.f);
		glScalef(m_TextScale, m_TextScale, 1.f);
		m_pButtonText->Draw();
	}
	glPopMatrix();
}

void Button::Select(bool select)
{
	if (select)
	{
		m_ButtonState = ButtonState::ON_SELECTION;
	}
	else
	{
		m_ButtonState = ButtonState::ON_DESELECTION;
	}
}

void Button::SetActive(bool active)
{
	if (active)
	{
		m_MoveDirection = m_DefaultPosition - m_MovePosition;
		m_ButtonState = ButtonState::ON_ACTIVATION;
	}
	else
	{
		m_MoveDirection = m_MovePosition - m_DefaultPosition;
		m_ButtonState = ButtonState::ON_DEACTIVATION;
	}
	m_MoveDirection = m_MoveDirection.Normalized() * m_ActivationSpeed;
}

bool Button::IsSelected() const
{
	return m_ButtonState == ButtonState::SELECTED;
}