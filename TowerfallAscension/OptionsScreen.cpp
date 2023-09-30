#include "pch.h"
#include "OptionsScreen.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Texture.h"
#include "Text.h"
#include "ScrollButton.h"
#include "Keyboard.h"

OptionsScreen::OptionsScreen(float windowWidth, float windowHeight, const ResourceManager* resources)
    :Screen(windowWidth, windowHeight, resources, Screen::ScreenType::OPTIONS)
{
	Vector2f scale{ m_Scale.x * 3.9f, m_Scale.x * 3.9f };
	TextureManager* textureManager = resources->GetTextureManager();
	Texture* pBackground = textureManager->GetTexture(TextureManager::TextureSheet::START_BACKGROUND);
	m_pBackground = new AnimatedSprite(pBackground, Rectf{ 0.f, pBackground->GetHeight() / 4.f,
		pBackground->GetWidth(), pBackground->GetHeight() / 4.f }, 4, 1, 5, 4,
		AnimatedSprite::AnimationState::LOOP);
	m_pBackground->SetScale(
		Vector2f{ m_WindowWidth / m_pBackground->GetWidth(), m_WindowHeight / m_pBackground->GetHeight() }
	);

	m_pTower = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::OPTIONS_TOWER));
	m_pTower->SetScale(scale);

	m_pTitle = new Text("Options", int(Text::Size::LARGE), resources);
	m_pButtons.push_back(new ScrollButton(Point2f{ m_WindowWidth * 0.64f, m_WindowHeight * 0.65f }, resources, std::string{ "SFX" }, 0, 128, 
		resources->GetSoundManager()->GetSoundEffectVolume()));
	m_pButtons.push_back(new ScrollButton(Point2f{ m_WindowWidth * 0.64f, m_WindowHeight * 0.55f }, resources, std::string{ "Music" }, 0, 128,
		resources->GetSoundManager()->GetTrackVolume()));

	m_pButtons[m_SelectedButton]->SetActive(true);

	resources->GetSoundManager()->StopTrack();
	m_pResources->GetSoundManager()->PlayTrack(SoundManager::Music::ARCHIVES);
}

OptionsScreen::~OptionsScreen()
{
	delete m_pTower;
	m_pTower = nullptr;

	delete m_pBackground;
	m_pBackground = nullptr;

	delete m_pTitle;
	m_pTitle = nullptr;
}

void OptionsScreen::Update(float elapsedSec)
{
	for (size_t i{}; i < m_pButtons.size(); ++i)
	{
		m_pButtons[i]->Update(elapsedSec);
	}
	m_pBackground->Update(elapsedSec);
}

void OptionsScreen::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_Translation.x, m_Translation.y, 0.f);
		m_pBackground->Draw();
		DrawTower();
		DrawTitle();
		DrawButtons();
	}
	glPopMatrix();
}

void OptionsScreen::ChangeOption(Option option)
{
	switch (option)
	{
	case Option::SOUND_SFX:		
		m_pResources->GetSoundManager()->SetSoundEffectVolume(m_pButtons[m_SelectedButton]->GetButtonValue());
		break;
	case Option::SOUND_MUSIC:
		m_pResources->GetSoundManager()->SetTrackVolume(m_pButtons[m_SelectedButton]->GetButtonValue());
		break;
	}
}

void OptionsScreen::DrawTower() const
{
	glPushMatrix();
	{
		glTranslatef(m_WindowWidth / 2.f - m_pTower->GetScaledWidth() / 2.f, 0.f, 0.f);
		m_pTower->Draw();
	}
	glPopMatrix();
}

void OptionsScreen::DrawTitle() const
{
	glPushMatrix();
	{
		glTranslatef(m_WindowWidth / 2.f - m_pTitle->GetTextWidth() / 2.f, m_WindowHeight - m_pTitle->GetTextHeight() * 3.f, 0.f);
		m_pTitle->Draw();
	}
	glPopMatrix();
}

void OptionsScreen::DrawButtons() const
{
	glPushMatrix();
	{
		for (size_t i{}; i < m_pButtons.size(); ++i)
		{
			m_pButtons[i]->Draw();
		}
	}
	glPopMatrix();
}

void OptionsScreen::ProcessKeyPress(const Keyboard* pKeyboard)
{
	for (size_t i{}; i < m_pButtons.size(); ++i)
	{
		m_pButtons[i]->SetPressed(false);
	}
	if (pKeyboard->IsMoveLeftPressed())
	{
		m_pButtons[m_SelectedButton]->AddButtonValue(- 1);
		ChangeOption(Option(m_SelectedButton));
	}
	else if (pKeyboard->IsMoveRightPressed())
	{
		m_pButtons[m_SelectedButton]->AddButtonValue(1);
		ChangeOption(Option(m_SelectedButton));
	}
	else if (pKeyboard->IsUpPressed())
	{
		m_pButtons[m_SelectedButton]->SetActive(false);
		--m_SelectedButton;
		if (m_SelectedButton < 0)
		{
			m_SelectedButton = int(m_pButtons.size()) - 1;
		}
		m_pButtons[m_SelectedButton]->SetActive(true);
	}
	else if (pKeyboard->IsCrouchPressed())
	{
		m_pButtons[m_SelectedButton]->SetActive(false);
		++m_SelectedButton;
		if (size_t(m_SelectedButton) >= m_pButtons.size())
		{
			m_SelectedButton = 0;
		}
		m_pButtons[m_SelectedButton]->SetActive(true);
	}
	else if (pKeyboard->IsShootPressed())
	{
		m_pButtons[m_SelectedButton]->SetPressed(true);
	}
}

bool OptionsScreen::IsKeyPressed() const
{
    return m_pButtons[m_SelectedButton]->IsPressed();
}

int OptionsScreen::GetButtonValue() const
{
    return int(ScreenType::START);
}


