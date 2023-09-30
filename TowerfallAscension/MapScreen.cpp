#include "pch.h"
#include "MapScreen.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "MapPointer.h"
#include "Keyboard.h"
#include "SoundManager.h"
#include "Text.h"

MapScreen::MapScreen(float windowWidth, float windowHeight, const std::vector<Vector2f>& locations, const std::vector<std::string>& levelNames, const ResourceManager* resources)
	:Screen(windowWidth, windowHeight, resources, ScreenType::MAP)
	,m_NextSelectedButton{0}
	,m_MapState{MapState::IDLE}
{
	for (size_t i{}; i < locations.size(); ++i)
	{
		m_pButtons.push_back(new MapPointer(Point2f{ locations[i].x * m_Scale.x * 5.f, locations[i].y * m_Scale.y * 5.f}, resources, 1.f, int(i)));
	}
	m_pButtons[m_SelectedButton]->Select(true);
	TextureManager* textureManager = resources->GetTextureManager();
	m_pBackground = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::MAP_BACKGROUND));
	m_pBackground->SetScale(m_Scale * 5.f);
	m_pResources->GetSoundManager()->StopTrack();
	m_pResources->GetSoundManager()->PlayTrack(SoundManager::Music::ARCHIVES);

	m_LevelNames = levelNames;
	m_pText = new Text(m_LevelNames[m_SelectedButton], int(Text::Size::LARGE), resources);
}

MapScreen::~MapScreen()
{
	delete m_pBackground;
	m_pBackground = nullptr;

	delete m_pText;
	m_pText = nullptr;
}

void MapScreen::Update(float elapsedSec)
{
	m_pButtons[m_SelectedButton]->Update(elapsedSec);
	switch (m_MapState)
	{
	case MapState::POINTER_TRANSITION:
		if (!m_pButtons[m_SelectedButton]->IsMoving())
		{
			m_MapState = MapState::IDLE;
			m_pButtons[m_SelectedButton]->Select(false);
			m_SelectedButton = m_NextSelectedButton;
			m_pButtons[m_SelectedButton]->Select(true);
			m_pText->SetNewText(m_LevelNames[m_SelectedButton], int(Text::Size::LARGE));
		}
		break;
	default:
		break;
	}
		
}

void MapScreen::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_Translation.x, m_Translation.y, 0.f);
		m_pBackground->Draw();
		m_pText->Draw();
		m_pButtons[m_SelectedButton]->Draw();
	}
	glPopMatrix();
}

void MapScreen::DrawHud() const
{
	glPushMatrix();
	{
		glTranslatef(m_Translation.x, m_Translation.y + m_WindowHeight - m_pText->GetTextHeight(), 0.f);
		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.5f });
		utils::FillRect(0.f, 0.f, m_WindowWidth, m_pText->GetTextHeight());
		glTranslatef(+ m_WindowWidth / 2.f - m_pText->GetTextWidth() / 2.f, 0.f, 0.f);
		m_pText->Draw();
	}
	glPopMatrix();
}

void MapScreen::ProcessKeyPress(const Keyboard* pKeyboard)
{
	switch (m_MapState)
	{
	case MapState::IDLE:
		m_IsEscPressed = false;
		for (size_t i{}; i < m_pButtons.size(); ++i)
		{
			m_pButtons[i]->SetPressed(false);
		}
		if (pKeyboard->IsMoveLeftPressed())
		{
			--m_NextSelectedButton;
			if (m_NextSelectedButton < 0)
			{
				m_NextSelectedButton = 0;
			}
			if (m_NextSelectedButton != m_SelectedButton)
			{
				m_MapState = MapState::POINTER_TRANSITION;
				m_pButtons[m_SelectedButton]->SetMovePosition(m_pButtons[m_NextSelectedButton]->GetPosition());
			}
		}
		else if (pKeyboard->IsMoveRightPressed())
		{
			++m_NextSelectedButton;
			if (m_NextSelectedButton >= int(m_pButtons.size()))
			{
				m_NextSelectedButton = int(m_pButtons.size()) - 1;
			}
			if (m_NextSelectedButton != m_SelectedButton)
			{
				m_MapState = MapState::POINTER_TRANSITION;
				m_pButtons[m_SelectedButton]->SetMovePosition(m_pButtons[m_NextSelectedButton]->GetPosition());
			}					
		}
		if (pKeyboard->IsJumpPressed())
		{

			if (m_pButtons[m_SelectedButton]->IsSelected())
			{
				m_pButtons[m_SelectedButton]->SetPressed(true);
			}
		}
		else if (pKeyboard->IsShootPressed())
		{
			m_IsEscPressed = true;
			m_pButtons[m_SelectedButton]->SetPressed(true);
		}
		break;
	case MapState::POINTER_TRANSITION:
		break;
	}
}

int MapScreen::GetButtonValue() const
{
	return m_IsEscPressed ? -1 : m_pButtons[m_SelectedButton]->GetButtonValue();
}

bool MapScreen::IsKeyPressed() const
{
	return m_pButtons[m_SelectedButton]->IsPressed();
}

Rectf MapScreen::GetMenuBoundaries() const
{
	Rectf boundaries{};
	boundaries.left = m_Translation.x;
	boundaries.bottom = m_Translation.y;
	boundaries.width = m_pBackground->GetScaledWidth();
	boundaries.height = m_pBackground->GetScaledHeight();
	return boundaries;
}

Point2f MapScreen::GetWidgetPosition() const
{
	return m_pButtons[m_SelectedButton]->GetPosition() + Vector2f{ m_Translation.x, 0.f };
}
