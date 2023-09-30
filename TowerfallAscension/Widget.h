#pragma once
#include "utils.h"
#include "Screen.h"
class ResourceManager;
class Widget
{
public:
	Widget(const Point2f& location, const ResourceManager* resources, float scale = 1.f, int value = int(Screen::ScreenType::NONE), bool isClickable = true);
	virtual ~Widget() = default;
	Widget(const Widget& c) = delete;
	Widget& operator=(const Widget& rhs) = delete;
	Widget(Widget&& c) = delete;
	Widget& operator=(Widget&& rhs) = delete;

	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const = 0;

	virtual void SetMovePosition(const Point2f& newLocation);
	const Point2f& GetPosition() const;
	virtual void SetActive(bool active);
	bool IsActive() const;
	bool IsClickable() const;
	virtual void Select(bool select);
	virtual void Select(int select);
	virtual bool IsSelected() const;
	virtual int GetButtonValue() const;
	virtual void SetPressed(bool pressed);
	virtual bool IsPressed() const;
	virtual bool IsMoving() const;
	virtual void AddButtonValue(int value);

protected:
	bool m_IsPressed;
	const int m_Value;

	Point2f m_DefaultPosition;
	Point2f m_MovePosition;
	Point2f m_CurrentPosition;

	float m_Scale;
	bool m_IsActive;
	bool m_IsClickable;

	const ResourceManager* m_pResources;
};

