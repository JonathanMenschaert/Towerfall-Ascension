#include "pch.h"
#include "Widget.h"

Widget::Widget(const Point2f& location, const ResourceManager* resources, float scale, int value, bool isClickable)
	:m_DefaultPosition{location}
	,m_MovePosition{location}
	,m_CurrentPosition{location}
	,m_Scale{scale}
	,m_IsActive{false}
	,m_IsClickable{isClickable}
	,m_Value{value}
	,m_IsPressed{ false }
	,m_pResources{resources}
{
}

void Widget::SetMovePosition(const Point2f& newLocation)
{
	m_MovePosition = newLocation;
}

const Point2f& Widget::GetPosition() const
{
	return m_CurrentPosition;
}

void Widget::SetActive(bool active)
{
	m_IsActive = active;
}

bool Widget::IsActive() const
{
	return m_IsActive;
}

bool Widget::IsClickable() const
{
	return m_IsClickable;
}

void Widget::Select(bool select)
{
}

void Widget::Select(int select)
{
}

bool Widget::IsSelected() const
{
	return false;
}

int Widget::GetButtonValue() const
{
	return m_Value;
}

void Widget::SetPressed(bool pressed)
{
	m_IsPressed = pressed;
}

bool Widget::IsPressed() const
{
	return m_IsPressed;
}

bool Widget::IsMoving() const
{
	bool isFirstPosEqual{ m_CurrentPosition.x == m_DefaultPosition.x && m_CurrentPosition.y == m_DefaultPosition.y };
	bool isSecondPosEqual{ m_CurrentPosition.x == m_MovePosition.x && m_CurrentPosition.y == m_MovePosition.y };
	return isFirstPosEqual || isSecondPosEqual;
}

void Widget::AddButtonValue(int value)
{
}


