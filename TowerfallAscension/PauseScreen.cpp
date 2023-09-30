#include "pch.h"
#include "PauseScreen.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "ListButton.h"
#include <string>
#include "Keyboard.h"
#include "Text.h"
#include "SoundManager.h"

PauseScreen::PauseScreen(float windowWidth, float windowHeight, const ResourceManager* resources)
	:Screen(windowWidth, windowHeight, resources, ScreenType::PAUSE)
	, m_pListButton{ new ListButton(Point2f{0.f, 0.f}, resources, {"Resume", "Restart", "Map", "Quit"}, 0.75f)}
	, m_PauseState{PauseState::PLAY}
	, m_pPauseText{new Text("Paused", int(Text::Size::LARGE), resources)}
{
	TextureManager* textureManager{ resources->GetTextureManager() };
	m_pPauseBackground = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::PAUSE_BACKGROUND));
	m_pPauseBackground->SetScale(m_Scale * 4.f);

	m_pPauseArrow = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::PAUSE_ARROW));
	m_pPauseArrow->SetScale(m_Scale * 5.f);
}

PauseScreen::~PauseScreen()
{
	delete m_pPauseBackground;
	m_pPauseBackground = nullptr;

	delete m_pListButton;
	m_pListButton = nullptr;

	delete m_pPauseArrow;
	m_pPauseArrow = nullptr;

	delete m_pPauseText;
	m_pPauseText = nullptr;
}

void PauseScreen::Update(float elapsedSec)
{
}

void PauseScreen::Draw() const
{
	switch (m_PauseState)
	{
	case PauseState::PLAY:
		break;
	case PauseState::PAUSE:
		glPushMatrix();
		{
			glTranslatef(m_Translation.x, m_Translation.y, 0.f);
			utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.5f });
			utils::FillRect(0.f, 0.f, m_WindowWidth, m_WindowHeight);
			glTranslatef(m_WindowWidth / 2.f - m_pPauseBackground->GetScaledWidth() / 2.f,
				m_WindowHeight / 2.f - m_pPauseBackground->GetScaledHeight() / 2.f, 0.f);
			m_pPauseBackground->Draw();
			glTranslatef(m_pPauseBackground->GetScaledWidth() / 2.f, m_pPauseBackground->GetScaledHeight() * 0.65f, 0.f);
			m_pListButton->Draw();
			
			glTranslatef(0.f, m_pPauseArrow->GetScaledHeight(), 0.f);
			glPushMatrix();
			{
				glTranslatef(-m_pPauseArrow->GetScaledWidth() / 2.f, 0.f, 0.f);
				m_pPauseArrow->Draw();
			}
			glPopMatrix();
			glTranslatef(-m_pPauseText->GetTextWidth() / 2.f, m_pPauseText->GetTextHeight() / 2.f, 0.f);
			m_pPauseText->Draw();
		}
		glPopMatrix();
		break;
	}
}

void PauseScreen::ProcessKeyPress(const Keyboard* pKeyboard)
{
	m_pListButton->SetPressed(false);
	m_IsEscPressed = false;
	switch (m_PauseState)
	{
	case PauseState::PLAY:
		if (pKeyboard->IsKeyPressed(Keyboard::Action::ESC))
		{
			m_PauseState = PauseState::PAUSE;			
			m_pResources->GetSoundManager()->PauseAllSoundEffects();
			m_pResources->GetSoundManager()->PauseTrack();
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::UI_PAUSE);
			m_pListButton->SetPressed(true);
			m_IsEscPressed = true;
		}
		break;
	case PauseState::PAUSE:
		if (pKeyboard->IsKeyPressed(Keyboard::Action::ESC))
		{
			m_PauseState = PauseState::PLAY;
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::UI_UNPAUSE);
			m_pResources->GetSoundManager()->ResumeTrack();
			m_pResources->GetSoundManager()->ResumeAllSoundEffects();
			m_pListButton->SetPressed(true);
			m_IsEscPressed = true;
		}
		if (pKeyboard->IsCrouchPressed())
		{
			m_pListButton->Select(1);
		}
		else if (pKeyboard->IsUpPressed())
		{
			m_pListButton->Select(-1);
		}
		if (pKeyboard->IsJumpPressed())
		{
			m_PauseState = PauseState::PLAY;
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::UI_UNPAUSE);
			m_pResources->GetSoundManager()->ResumeAllSoundEffects();
			m_pListButton->SetPressed(true);
		}
	}
}

int PauseScreen::GetButtonValue() const
{
	return m_IsEscPressed ? int(ListValue::ESCAPE) : m_pListButton->GetButtonValue();
}

bool PauseScreen::IsKeyPressed() const
{
	return m_pListButton->IsPressed();
}
