#include "pch.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "ScrollButton.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "Sprite.h"
#include <string>
#include "Text.h"

ScrollButton::ScrollButton(const Point2f& location, const ResourceManager* resources, const std::string& text, int min, int max, int current, float scale)
    :Widget(location, resources, scale)
    , m_CurrentValue{current}
    , m_MinValue{min}
    , m_MaxValue{max}
    , m_ArrowAngle{0.f}
    , m_ArrowRotSpeed{360.f}
    , m_pName{new Text(text, int(Text::Size::MEDIUM), resources)}
    , m_OpenSpace{300.f}
{
    const Vector2f scaler{ 3.5f * scale, 3.5f * scale};
    TextureManager* textureManager{ resources->GetTextureManager() };
    m_pScrollButton = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::BUTTON_SCROLL));
    m_pScrollButton->SetScale(scaler);
    m_pValue = new Text(std::to_string(current), int(Text::Size::MEDIUM), resources);
}

ScrollButton::~ScrollButton()
{
    delete m_pScrollButton;
    m_pScrollButton = nullptr;

    delete m_pValue;
    m_pValue = nullptr;

    delete m_pName;
    m_pName = nullptr;
}

void ScrollButton::Update(float elapsedSec)
{
    if (m_IsActive)
    {
        if (m_ArrowAngle <= -180.f)
        {
            m_ArrowAngle = 180.f;
        }
        else
        {
            m_ArrowAngle -= m_ArrowRotSpeed * elapsedSec;
        }
    }
}

void ScrollButton::Draw() const
{
    glPushMatrix();
    {
        glTranslatef(m_CurrentPosition.x, m_CurrentPosition.y - m_pValue->GetTextHeight() / 2.f, 0.f);
        DrawName();
        glTranslatef(-m_pValue->GetTextWidth() / 2.f, 0.f, 0.f);
        m_pValue->Draw();
        if (m_IsActive)
        {
            glTranslatef(m_pValue->GetTextWidth() / 2.f, 0.f, 0.f);
            if (m_CurrentValue < m_MaxValue) {
                DrawArrow(1);
            }
            if (m_CurrentValue > m_MinValue)
            {
                DrawArrow(-1);
            }
        
        }
    }
    glPopMatrix();
}

void ScrollButton::Select(int select)
{
    m_IsActive = select;
}

int ScrollButton::GetButtonValue() const
{
    return m_CurrentValue;
}

void ScrollButton::AddButtonValue(int value)
{
    if ((value < 0 && m_CurrentValue > m_MinValue) || (value > 0 && m_CurrentValue < m_MaxValue))
    {
        m_CurrentValue += value;
        m_pValue->SetNewText(std::to_string(m_CurrentValue), int(Text::Size::MEDIUM));
    }
}

void ScrollButton::DrawArrow(int dir) const
{
    glPushMatrix();
    {
        glTranslatef(dir * m_pScrollButton->GetScaledWidth() * 2.f + 
            cosf(float(M_PI) / 180.f * (m_ArrowAngle + 90 * dir)) * m_pScrollButton->GetScaledWidth() / 3.f, 0.f, 0.f);
        glScalef(float(dir), 1.f, 1.f);
        m_pScrollButton->Draw();
    }
    glPopMatrix();
}

void ScrollButton::DrawName() const
{
    glPushMatrix();
    {
        glTranslatef(-m_OpenSpace, 0.f, 0.f);
        m_pName->Draw();
    }
    glPopMatrix();
}
