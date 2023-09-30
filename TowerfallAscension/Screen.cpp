#include "pch.h"
#include "Screen.h"
#include "Widget.h"
#include "Keyboard.h"
#include "Button.h"

Screen::Screen(float windowWidth, float windowHeight, const ResourceManager* resources, ScreenType type)
	: m_Scale{ windowWidth / 1920.f, windowHeight / 1080.f }
	, m_SelectedButton{0}
	, m_MenuType{type}
	,m_IsEscPressed{false}
	,m_pResources{resources}
{
	
	m_Translation = Vector2f{ (1920.f - 1440.f) / 2.f * m_Scale.x, 0.f };
	m_WindowWidth = windowWidth - 2 * m_Translation.x;
	m_WindowHeight = windowHeight - 2 * m_Translation.y;
}

Screen::~Screen()
{
	for (size_t i{}; i < m_pButtons.size(); ++i)
	{
		delete m_pButtons[i];
	}
	m_pButtons.clear();
}

void Screen::DrawHud() const
{
}

bool Screen::IsKeyPressed() const
{
	return false;
}

Rectf Screen::GetMenuBoundaries() const
{
	Rectf boundaries{};
	boundaries.left = m_Translation.x;
	boundaries.bottom = m_Translation.y;
	boundaries.width = m_WindowWidth;
	boundaries.height = m_WindowHeight;
	return boundaries;
}

Point2f Screen::GetWidgetPosition() const
{
	return Point2f{};
}

bool Screen::IsReady() const
{
	return true;
}

Screen::ScreenType Screen::GetMenuType() const
{
	return m_MenuType;
}
