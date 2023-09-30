#include "pch.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "StartScreen.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Text.h"
#include "Widget.h"
#include "ButtonStart.h"
#include "Keyboard.h"
#include "SoundManager.h"
#include "OptionButton.h"


StartScreen::StartScreen(float windowWidth, float windowHeight, const ResourceManager* resources)
	:Screen(windowWidth, windowHeight, resources, ScreenType::START)
	,m_LetterAngle{}
	,m_LetterRotationSpeed{120.f}
	,m_LetterRotationInterval{15.f}
	,m_State{ScreenState::TITLE}
	,m_JitterInterval{}
	,m_ShouldTextShow{true}
	,m_MaxJitterTime{1.f}
	,m_MaxJitterInterval{0.05f}
	,m_JitterTime{}
{
	Vector2f scale{ m_Scale.x * 3.9f, m_Scale.x * 3.9f};
	TextureManager* textureManager = resources->GetTextureManager();
	Texture* pBackground = textureManager->GetTexture(TextureManager::TextureSheet::START_BACKGROUND);
	m_pBackground = new AnimatedSprite(pBackground, Rectf{ 0.f, pBackground->GetHeight() / 4.f, 
		pBackground->GetWidth(), pBackground->GetHeight() / 4.f }, 4, 1, 5, 4,
		AnimatedSprite::AnimationState::LOOP);
	m_pBackground->SetScale(
		Vector2f{m_WindowWidth / m_pBackground->GetWidth(), m_WindowHeight / m_pBackground->GetHeight()}
	);

	m_pTower = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::START_TOWER));
	m_pTower->SetScale(scale);

	m_pArrow = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::START_ARROW));
	m_pArrow->SetScale(scale);

	m_pOrbBottom = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::START_ORBBOTTOM));
	m_pOrbBottom->SetScale(scale);

	m_pBanner = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::START_BANNER));
	m_pBanner->SetScale(scale);	

	m_pTitle = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::START_TITLE));
	m_pTitle->SetScale(scale);

	Texture* pOrb = textureManager->GetTexture(TextureManager::TextureSheet::START_ORB);
	m_pOrb = new AnimatedSprite(pOrb, Rectf{0.f, pOrb->GetHeight(), pOrb->GetWidth() / 8.f, pOrb->GetHeight()},
		1, 8, 8, 8, AnimatedSprite::AnimationState::LOOP);
	m_pOrb->SetScale(scale);

	Texture* pLetters = textureManager->GetTexture(TextureManager::TextureSheet::START_LETTERS);

	m_pLetters.push_back(new Sprite(pLetters, Rectf{ 0.f, pLetters->GetHeight(), pLetters->GetWidth() / 7.f, pLetters->GetHeight() }));
	m_pLetters.push_back(new Sprite(pLetters, Rectf{ pLetters->GetWidth() / 7.f, pLetters->GetHeight(), pLetters->GetWidth() / 7.f, pLetters->GetHeight() }));
	m_pLetters.push_back(new Sprite(pLetters, Rectf{ pLetters->GetWidth() / 3.5f, pLetters->GetHeight(), pLetters->GetWidth() / 7.f, pLetters->GetHeight() }));
	m_pLetters.push_back(new Sprite(pLetters, Rectf{ pLetters->GetWidth() / 7.f * 3.f, pLetters->GetHeight(), pLetters->GetWidth() / 7.f, pLetters->GetHeight() }));
	m_pLetters.push_back(new Sprite(pLetters, Rectf{ pLetters->GetWidth() / 7.f * 4.f, pLetters->GetHeight(), pLetters->GetWidth() / 7.f, pLetters->GetHeight() }));
	m_pLetters.push_back(new Sprite(pLetters, Rectf{ pLetters->GetWidth() / 7.f, pLetters->GetHeight(), pLetters->GetWidth() / 7.f, pLetters->GetHeight() }));
	m_pLetters.push_back(new Sprite(pLetters, Rectf{ pLetters->GetWidth() / 7.f * 5.f, pLetters->GetHeight(), pLetters->GetWidth() / 7.f, pLetters->GetHeight() }));
	m_pLetters.push_back(new Sprite(pLetters, Rectf{ pLetters->GetWidth() / 7.f * 6.f, pLetters->GetHeight(), pLetters->GetWidth() / 7.f, pLetters->GetHeight() }));
	m_pLetters.push_back(new Sprite(pLetters, Rectf{ pLetters->GetWidth() / 7.f * 4.f, pLetters->GetHeight(), pLetters->GetWidth() / 7.f, pLetters->GetHeight() }));

	for (size_t i{}; i < m_pLetters.size(); ++i)
	{
		m_pLetters[i]->SetScale(scale);
	}

	m_pText = new Text("Press Space key to continue", int(Text::Size::SMALL), m_pResources);
	const float heightModifier{ 0.4f };
	m_pButtons.push_back(new ButtonStart(Point2f{ windowWidth * 0.33f, windowHeight * heightModifier }, Point2f{ 0.f, windowHeight * heightModifier}, m_pResources, 2.5f));
	m_pButtons.push_back(new OptionButton(Point2f{ windowWidth * 0.66f, windowHeight * heightModifier }, Point2f{ windowWidth, windowHeight * heightModifier }, m_pResources, 2.5f));

	resources->GetSoundManager()->StopTrack();
	m_pResources->GetSoundManager()->PlayTrack(SoundManager::Music::MAINMENU);
}

StartScreen::~StartScreen()
{
	delete m_pBackground;
	m_pBackground = nullptr;

	delete m_pArrow;
	m_pArrow = nullptr;

	delete m_pOrbBottom;
	m_pOrbBottom = nullptr;

	delete m_pBanner;
	m_pBanner = nullptr;

	delete m_pTower;
	m_pTower = nullptr;

	delete m_pTitle;
	m_pTitle = nullptr;

	delete m_pOrb;
	m_pOrb = nullptr;

	for (size_t i{}; i < m_pLetters.size(); ++i)
	{
		delete m_pLetters[i];
	}
	m_pLetters.clear();

	delete m_pText;
	m_pText = nullptr;
}

void StartScreen::Update(float elapsedSec)
{
	m_pOrb->Update(elapsedSec);
	m_pBackground->Update(elapsedSec);
	switch (m_State)
	{		
	case ScreenState::TRANSITION_MENU:
	{
		if (m_JitterTime < m_MaxJitterTime)
		{
			m_JitterTime += elapsedSec;
			m_JitterInterval += elapsedSec;

			if (m_JitterInterval >= m_MaxJitterInterval)
			{
				m_ShouldTextShow = !m_ShouldTextShow;
				m_JitterInterval -= m_MaxJitterInterval;
			}
		}
		else
		{
			bool areButtonsReady{ true };
			for (size_t i{}; i < m_pButtons.size(); ++i)
			{
				m_pButtons[i]->Update(elapsedSec);
				areButtonsReady &= m_pButtons[i]->IsActive();
			}
			if (areButtonsReady)
			{
				m_JitterTime = 0.f;
				m_State = ScreenState::MENU;
				m_pButtons[0]->Select(true);
			}
		}
	}
	case ScreenState::TITLE:
	{
		if (m_LetterAngle <= -360.f)
		{
			m_LetterAngle = 0.f;
		}
		else
		{
			m_LetterAngle -= m_LetterRotationSpeed * elapsedSec;
		}
	}
		break;	
	case ScreenState::MENU:
		for (size_t i{}; i < m_pButtons.size(); ++i)
		{
			m_pButtons[i]->Update(elapsedSec);
		}
		break;
	case ScreenState::TRANSITION_TITLE:
	{
		bool areButtonsDisabled{ false };
		for (size_t i{}; i < m_pButtons.size(); ++i)
		{
			m_pButtons[i]->Update(elapsedSec);
			areButtonsDisabled |= !m_pButtons[i]->IsActive();
		}
		if (areButtonsDisabled)
		{
			m_State = ScreenState::TITLE;
		}
	}
	break;
	}
}
void StartScreen::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_Translation.x, m_Translation.y, 0.f);
		DrawBackground();
		DrawTower();
		glTranslatef(0.f, m_pTower->GetScaledHeight() * 0.8f, 0.f);
		DrawBanner();
		DrawArrow();
		DrawOrb();
		DrawTitle();
		if (m_State == ScreenState::TITLE || m_State == ScreenState::TRANSITION_MENU)
		{
			DrawLetters();
			glTranslatef(0.f, -m_WindowHeight * 0.6f, 0.f);
			if (m_ShouldTextShow)
			{
				DrawMenuText();
			}
		}			
	}
	glPopMatrix();
	if (m_State != ScreenState::TITLE)
	{
		DrawButtons();
	}	
}

void StartScreen::DrawBackground() const
{
	m_pBackground->Draw();
}

void StartScreen::DrawOrb() const
{
	glPushMatrix();
	{
		glTranslatef(m_WindowWidth / 2.f - m_pBanner->GetWidth() * 0.58f, -m_pBanner->GetHeight() * 1.36f, 0.f);
		m_pOrbBottom->Draw();
		glTranslatef(-m_pOrb->GetWidth() * 0.57f, m_pBanner->GetHeight() * 1.77f, 0.f);
		m_pOrb->Draw();
	}
	glPopMatrix();	
}

void StartScreen::DrawTower() const
{
	glPushMatrix();
	{
		glTranslatef(m_WindowWidth / 2.f - m_pTower->GetScaledWidth() / 2.f, 0.f, 0.f);
		m_pTower->Draw();
	}
	glPopMatrix();
}

void StartScreen::DrawBanner() const
{
	glPushMatrix();
	{
		glTranslatef(m_WindowWidth / 2.f - m_pBanner->GetScaledWidth() / 2.f, -m_pBanner->GetScaledHeight() / 2.f, 0.f);
		m_pBanner->Draw();
	}
	glPopMatrix();
}

void StartScreen::DrawTitle() const
{
	glPushMatrix();
	{
		glTranslatef(m_WindowWidth / 2.f - m_pTitle->GetScaledWidth() / 2.f, -m_pTitle->GetScaledHeight() / 2.f, 0.f);
		m_pTitle->Draw();
	}
	glPopMatrix();
	
}

void StartScreen::DrawArrow() const
{
	glPushMatrix();
	{
		glTranslatef(m_WindowWidth / 2.f - m_pArrow->GetScaledWidth() / 2.f, -m_pArrow->GetScaledHeight() / 2.f, 0.f);
		m_pArrow->Draw();
	}
	glPopMatrix();
}

void StartScreen::DrawLetters() const
{
	glPushMatrix();
	{
		glTranslatef(2.1f * m_pLetters[0]->GetScaledWidth(), -1.3f * m_pLetters[0]->GetScaledHeight(), 0.f);
		for (size_t i{}; i < m_pLetters.size(); ++i)
		{
			glPushMatrix();
			{
				glTranslatef(0.f, m_pLetters[i]->GetHeight() / 4.f * sinf(float(M_PI) / 180.f * (m_LetterAngle + i * m_LetterRotationInterval)), 0.f);
				m_pLetters[i]->Draw();
			}
			glPopMatrix();			
			glTranslatef(m_pLetters[i]->GetScaledWidth() * 0.65f, 0.f, 0.f);
			
		}
	}
	glPopMatrix();
}

void StartScreen::DrawMenuText() const
{
	glPushMatrix();
	{
		glTranslatef(m_WindowWidth / 2.f - m_pText->GetTextWidth() / 2.f, 0.f, 0.f);
		m_pText->Draw();
	}
	glPopMatrix();
	
}

void StartScreen::ProcessKeyPress(const Keyboard* pKeyboard)
{
	switch (m_State)
	{
	case ScreenState::TITLE:
		if (pKeyboard->IsJumpPressed())
		{
			m_State = ScreenState::TRANSITION_MENU;
			for (size_t i{}; i < m_pButtons.size(); ++i)
			{
				m_pButtons[i]->SetActive(true);
			}
		}
		break;
	case ScreenState::MENU:

		for (size_t i{}; i < m_pButtons.size(); ++i)
		{
			if (m_pButtons[i]->IsSelected())
			{
				m_SelectedButton = int(i);				
			}
			m_pButtons[i]->SetPressed(false);
		}

		if (pKeyboard->IsMoveLeftPressed())
		{
			m_pButtons[m_SelectedButton]->Select(false);
			--m_SelectedButton;
			if (m_SelectedButton < 0)
			{
				m_SelectedButton = int(m_pButtons.size()) - 1;
			}
			m_pButtons[m_SelectedButton]->Select(true);
		}
		if (pKeyboard->IsMoveRightPressed())
		{
			m_pButtons[m_SelectedButton]->Select(false);
			++m_SelectedButton;
			if (size_t(m_SelectedButton) >= m_pButtons.size())
			{
				m_SelectedButton = 0;
			}
			m_pButtons[m_SelectedButton]->Select(true);
		}
		if (pKeyboard->IsShootPressed())
		{
			m_State = ScreenState::TRANSITION_TITLE;
			for (size_t i{}; i < m_pButtons.size(); ++i)
			{
				m_pButtons[i]->SetActive(false);
			}
		}
		if (pKeyboard->IsJumpPressed())
		{
			
			if (m_pButtons[m_SelectedButton]->IsSelected())
			{
				m_pButtons[m_SelectedButton]->SetPressed(true);
			}
			
		}
		break;
	}
}

int StartScreen::GetButtonValue() const
{
	
	if (m_pButtons[m_SelectedButton]->IsPressed())
	{
		m_pButtons[m_SelectedButton]->SetPressed(false);
		return m_pButtons[m_SelectedButton]->GetButtonValue();
	}
	
	return int(Screen::ScreenType::NONE);
}

bool StartScreen::IsKeyPressed() const
{
	if (m_pButtons[m_SelectedButton]->IsSelected())
	{			
		if (m_pButtons[m_SelectedButton]->IsPressed())
		{				
			return true;
		}
	}
	return false;
}

void StartScreen::DrawButtons() const
{
	for (size_t i{}; i < m_pButtons.size(); ++i)
	{
		m_pButtons[i]->Draw();
	}
}
