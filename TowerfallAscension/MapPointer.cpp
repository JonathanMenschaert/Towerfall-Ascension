#include "pch.h"
#include "MapPointer.h"
#define _USE_MATH_DEFINES
#include <cmath>

MapPointer::MapPointer(const Point2f& location, const ResourceManager* resources, float scale, int value, bool isClickable)
    :Widget(location, resources, scale, value, isClickable)
    ,m_BobbingAngle{270.f}
    ,m_BobbingSpeed{150.f}
    ,m_MaxBobbingAngle{360.f}
    ,m_BobbingRadius{8.f}
    ,m_MoveVector{}
    , m_MaxMoveDistance{}
    ,m_MoveSpeed{5.f}
    ,m_PointerState{PointerState::HOVERING}
{
    TextureManager* textureManager = resources->GetTextureManager();
    m_pPointer = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::MAP_POINTER));
    m_pPointer->SetScale(Vector2f{ m_Scale * 3.5f, m_Scale * 3.5f});
}

MapPointer::~MapPointer()
{
    delete m_pPointer;
    m_pPointer = nullptr;
}


void MapPointer::Update(float elapsedSec)
{
    switch (m_PointerState)
    {
    case PointerState::HOVERING:
        m_BobbingAngle += m_BobbingSpeed * elapsedSec;
        if (m_BobbingAngle >= m_MaxBobbingAngle)
        {
            m_BobbingAngle -= m_MaxBobbingAngle;
        }
        break;
    case PointerState::MOVING:
        m_CurrentPosition += m_MoveVector * m_MoveSpeed;
        if (utils::GetDistance(m_DefaultPosition, m_CurrentPosition) >= m_MaxMoveDistance)
        {
            m_CurrentPosition = m_MovePosition;
            m_PointerState = PointerState::HOVERING;
        }
        break;
    }
}

void MapPointer::Draw() const
{
    glPushMatrix();
    {
        float heightOffset{};
        switch (m_PointerState)
        {
        case PointerState::HOVERING:
           heightOffset = sinf((float(M_PI) / 180.f * m_BobbingAngle)) * m_BobbingRadius;
           break;
        default:
            break;
        }
        glTranslatef(m_CurrentPosition.x - m_pPointer->GetScaledWidth() / 2.f, m_CurrentPosition.y + heightOffset, 0.f);
        m_pPointer->Draw();
    }
    glPopMatrix();
}

bool MapPointer::IsSelected() const
{
    return m_IsActive;
}

void MapPointer::SetPressed(bool pressed)
{
    m_IsPressed = pressed;
}

void MapPointer::SetMovePosition(const Point2f& newLocation)
{
    m_MovePosition = newLocation;
    m_MoveVector = Vector2f{ newLocation - m_DefaultPosition }.Normalized();
    m_MaxMoveDistance = utils::GetDistance(m_DefaultPosition, m_MovePosition);
    m_PointerState = PointerState::MOVING;
}

bool MapPointer::IsMoving() const
{
    return m_PointerState == PointerState::MOVING;
}

void MapPointer::Select(bool select)
{
    Widget::Select(select);
    if (!select)
    {
        m_CurrentPosition = m_DefaultPosition;
    }
    m_IsActive = select;
}
