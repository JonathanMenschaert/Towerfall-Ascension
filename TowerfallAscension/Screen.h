#pragma once
#include "utils.h"
#include <vector>
#include <typeinfo>

class Keyboard;
class Widget;
class ResourceManager;
class Screen
{
public:
	enum class ScreenType
	{		
		START,
		MAP,
		PAUSE,
		OPTIONS,
		ENDING,
		NONE
	};
	Screen(float windowWidth, float windowHeight, const ResourceManager* resources, ScreenType type = ScreenType::NONE);
	virtual ~Screen();
	Screen(const Screen& c) = delete;
	Screen& operator=(const Screen& rhs) = delete;
	Screen(Screen&& c) = delete;
	Screen& operator=(Screen&& rhs) = delete;
	
	Screen::ScreenType GetMenuType() const;

	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const = 0;
	virtual void DrawHud() const;
	virtual void ProcessKeyPress(const Keyboard* pKeyboard) = 0;
	virtual bool IsKeyPressed() const;
	virtual int GetButtonValue() const = 0;
	virtual Rectf GetMenuBoundaries() const;
	virtual Point2f GetWidgetPosition() const;
	virtual bool IsReady() const;

protected:
	int m_SelectedButton;
	float m_WindowWidth;
	float m_WindowHeight;
	bool m_IsEscPressed;

	std::vector<Widget*> m_pButtons;
	Vector2f m_Translation;
	Vector2f m_Scale;
	ScreenType m_MenuType;

	const ResourceManager* m_pResources;
	
};

