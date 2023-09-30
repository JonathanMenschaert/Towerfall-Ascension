#include "pch.h"
#include "EndScreen.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "ListButton.h"
#include "Text.h"
#include "Keyboard.h"

EndScreen::EndScreen(float windowWidth, float windowHeight, const ResourceManager* resources, bool victory)
    :Screen(windowWidth, windowHeight, resources, Screen::ScreenType::ENDING)
    , m_FallingLength{}
    , m_MaxFallingLength{550.f}
    , m_FallingWindowSpeed{0.f}
    , m_FallingWindowAcceleration{250.f}
    , m_EndStage{EndStage::BACKGROUND_APPEAR}
    , m_pListButton{ new ListButton(Point2f{0.f, 0.f}, resources, {"Restart", "Map", "Quit"}, 0.75f) }    
    , m_IsVictorious{victory}
    , m_DataTimer{}
    , m_MaxDataTimer{2.f}
{   
    m_FallingWindowSpeed = m_WindowHeight;

    Vector2f scale{ 3.f, 3.f };
    TextureManager* textureManager{ resources->GetTextureManager() };
    Texture* pBackground{textureManager->GetTexture(TextureManager::TextureSheet::END_BACKGROUND)};
    m_pUpperMenu = new Sprite(pBackground, Rectf{
        pBackground->GetWidth(), pBackground->GetHeight() / 2, pBackground->GetWidth(), pBackground->GetHeight() / 2.f
        });
    m_pUpperMenu->SetScale(scale);
    m_pLowerMenu = new Sprite(pBackground, Rectf{
        pBackground->GetWidth(), pBackground->GetHeight(), pBackground->GetWidth(), pBackground->GetHeight() / 2.f
        });
    m_pLowerMenu->SetScale(scale);

    m_pEndMenu = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::PAUSE_BACKGROUND));
    m_pEndMenu->SetScale(m_Scale * 4.f);

    m_pEndArrow = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::PAUSE_ARROW));
    m_pEndArrow->SetScale(m_Scale * 5.f);

    m_pEndPicture = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::END_PICTURE));
    m_pEndPicture->SetScale(scale);

    m_MenuWidth = (m_pLowerMenu->GetWidth() - 28.f) * scale.x;

    resources->GetSoundManager()->StopAllSoundEffects();
    resources->GetSoundManager()->StopTrack();
    if (victory)
    {
        m_pEndText = new Text("COMPLETED!", int(Text::Size::LARGE), resources);
        resources->GetSoundManager()->PlayTrack(SoundManager::Music::VICTORY);
    }
    else
    {
        m_pEndText = new Text("GAME OVER!", int(Text::Size::LARGE), resources);
        resources->GetSoundManager()->PlayTrack(SoundManager::Music::FALLEN);
    }
}

EndScreen::~EndScreen()
{
    delete m_pUpperMenu;
    m_pUpperMenu = nullptr;

    delete m_pLowerMenu;
    m_pLowerMenu = nullptr;
    
    delete m_pEndArrow;
    m_pEndArrow = nullptr;

    delete m_pEndText;
    m_pEndText = nullptr;

    delete m_pListButton;
    m_pListButton = nullptr;

    delete m_pEndMenu;
    m_pEndMenu = nullptr;

    delete m_pEndPicture;
    m_pEndPicture = nullptr;
}

void EndScreen::Update(float elapsedSec)
{
    switch (m_EndStage)
    {
    case EndStage::BACKGROUND_APPEAR:
        if (m_FallingLength >= m_MaxFallingLength)
        {
            m_EndStage = EndStage::DATA_APPEAR;
        }
        m_FallingWindowSpeed += m_FallingWindowAcceleration * elapsedSec;
        m_FallingLength += m_FallingWindowSpeed * elapsedSec;
        break;
    case EndStage::DATA_APPEAR:
        m_DataTimer += elapsedSec;
        if (m_DataTimer >= m_MaxDataTimer)
        {
            m_EndStage = EndStage::MENU_APPEAR;
        }
    case EndStage::MENU_APPEAR:
        break;
    default:
        break;
    }
}

void EndScreen::Draw() const
{
    glPushMatrix();
    {
        glTranslatef(m_Translation.x, m_Translation.y, 0.f);
        switch (m_EndStage)
        {
        case EndStage::BACKGROUND_APPEAR:
            DrawBackground();
            break;
        case EndStage::DATA_APPEAR:            
            DrawBackground();
            DrawData();
        break;
        case EndStage::MENU_APPEAR:
            DrawBackground();
            DrawData();
            DrawMenu();
        break;
        }
    }
    glPopMatrix();   
}

void EndScreen::ProcessKeyPress(const Keyboard* pKeyboard)
{
    m_pListButton->SetPressed(false);
    m_IsEscPressed = false;
    switch (m_EndStage)
    {
    case EndStage::MENU_APPEAR:
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
            m_pListButton->SetPressed(true);
        }
    }
}

bool EndScreen::IsKeyPressed() const
{
    return m_pListButton->IsPressed();
}

int EndScreen::GetButtonValue() const
{
    return m_pListButton->GetButtonValue();
}

void EndScreen::DrawData() const
{
    if (m_IsVictorious)
    {
        glPushMatrix();
        {
            glTranslatef(m_WindowWidth / 2.f - m_pEndPicture->GetScaledWidth() / 2.f, m_WindowHeight * 0.55f, 0.f);
            m_pEndPicture->Draw();
        }
        glPopMatrix();
    }
}

void EndScreen::DrawMenu() const
{   
    glPushMatrix();
    {
        glTranslatef(m_WindowWidth / 2.f - m_pEndMenu->GetScaledWidth() / 2.f, m_WindowHeight * 0.2f, 0.f);
        m_pEndMenu->Draw();
        glTranslatef(m_pEndMenu->GetScaledWidth() / 2.f, m_pEndMenu->GetScaledHeight() * 0.65f, 0.f);
        m_pListButton->Draw();
    }
    glPopMatrix();
}

void EndScreen::DrawBackground() const
{
    glPushMatrix();
    {
        utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.5f });
        utils::FillRect(0.f, 0.f, m_WindowWidth, m_WindowHeight);
        glTranslatef(m_WindowWidth / 2.f - m_pUpperMenu->GetScaledWidth() / 2.f, m_WindowHeight / 2.f - m_pUpperMenu->GetScaledHeight() / 2.f, 0.f);
        glTranslatef(0.f, m_WindowHeight / 2.5f, 0.f);

        glPushMatrix();
        {
            glTranslatef(0.f, -m_FallingLength, 0.f);
            utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
            utils::FillRect((m_pLowerMenu->GetScaledWidth() - m_MenuWidth) / 2.f, 0.f, m_MenuWidth, m_FallingLength);
            m_pLowerMenu->Draw();
        }
        glPopMatrix();

        m_pUpperMenu->Draw();
        DrawArrow();
    }
    glPopMatrix();
}

void EndScreen::DrawArrow() const
{
    glPushMatrix();
    {
        glTranslatef(0.f, -m_pEndArrow->GetScaledHeight() * 0.75f, 0.f);
        m_pEndArrow->Draw();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(m_pEndArrow->GetScaledWidth() / 2.f - m_pEndText->GetTextWidth() / 2.f, -m_pEndText->GetTextHeight(), 0.f);
        m_pEndText->Draw();
    }
    glPopMatrix();
}
