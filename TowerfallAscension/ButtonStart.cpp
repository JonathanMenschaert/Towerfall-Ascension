#include "pch.h"
#include "ButtonStart.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Text.h"

ButtonStart::ButtonStart(const Point2f& location, const Point2f& startLocation, const ResourceManager* resources, float scale)
	:Button(resources->GetTextureManager()->GetTexture(TextureManager::TextureSheet::BUTTON_START), 
		location, startLocation, "Quest", Text::Size::MEDIUM, Screen::ScreenType::MAP, resources, Vector2f{0.f, -100.f}, scale)
	, m_RotSpeed{ 720.f }
{
}

void ButtonStart::Update(float elapsedSec)
{
	switch (m_ButtonState)
	{
	case ButtonState::ON_ACTIVATION:
		m_CurrentPosition += m_MoveDirection * elapsedSec;
		if (utils::GetDistance(m_CurrentPosition, m_MovePosition) >= m_ActivationDistance)
		{
			m_CurrentPosition = m_DefaultPosition;
			m_ButtonState = ButtonState::DESELECTED;
			Widget::SetActive(true);
		}
		break;
	case ButtonState::ON_SELECTION:
	{
		bool isFinished{ true };
		if (m_Rotation < m_MaxRotation)
		{
			m_Rotation += m_RotSpeed * elapsedSec;
			isFinished = false;
		}
		if (m_TextScale < m_MaxTextScale)
		{
			m_TextScale += m_ScaleSpeed * elapsedSec;
			isFinished = false;
		}
		if (isFinished)
		{
			m_Rotation = m_MaxRotation;
			m_TextScale = m_MaxTextScale;
			m_ButtonState = ButtonState::SELECTED;
		}
	}
	case ButtonState::SELECTED:
		m_BobbingAngle += m_BobbingSpeed * elapsedSec;
		break;
	case ButtonState::ON_DESELECTION:
	{
		bool isFinished{ true };
		if (m_Rotation > 0.f)
		{
			m_Rotation -= m_RotSpeed * elapsedSec;
			isFinished = false;
		}
		if (m_TextScale > m_MinTextScale)
		{
			m_TextScale -= m_ScaleSpeed * elapsedSec;
			isFinished = false;
		}
		if (isFinished)
		{
			m_ButtonState = ButtonState::DESELECTED;
		}
	}
		break;
	case ButtonState::DESELECTED:
		//Lock state to deselected, do nothing
		break;
	case ButtonState::ON_DEACTIVATION:
		m_CurrentPosition += m_MoveDirection * elapsedSec;
		if (utils::GetDistance(m_CurrentPosition, m_DefaultPosition) >= m_ActivationDistance)
		{
			m_CurrentPosition = m_MovePosition;
			Widget::SetActive(false);
			m_Rotation = 0.f;
			m_TextScale = m_MinTextScale;
			m_ButtonState = ButtonState::DEACTIVATED;
		}
		break;
	}
}