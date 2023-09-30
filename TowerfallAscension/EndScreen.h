#pragma once
#include "Screen.h"
class Sprite;
class ListButton;
class Text;
class EndScreen final : public Screen
{
public:
	enum class EndStage
	{
		BACKGROUND_APPEAR,
		DATA_APPEAR,
		MENU_APPEAR
	};

	enum class ListValue
	{
		RESTART,
		MAP,
		QUIT,
	};
	EndScreen(float windowWidth, float windowHeight, const ResourceManager* resources, bool victory);
	virtual ~EndScreen();
	EndScreen(const EndScreen& c) = delete;
	EndScreen& operator=(const EndScreen& rhs) = delete;
	EndScreen(EndScreen&& c) = delete;
	EndScreen& operator=(EndScreen&& rhs) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	virtual void ProcessKeyPress(const Keyboard* pKeyboard) override;
	virtual bool IsKeyPressed() const override;
	virtual int GetButtonValue() const override;

private:
	
	bool m_IsVictorious;

	float m_DataTimer;
	const float m_MaxDataTimer;

	float m_FallingWindowSpeed;
	float m_FallingLength;
	float m_MenuWidth;

	const float m_FallingWindowAcceleration;
	const float m_MaxFallingLength; 

	EndStage m_EndStage;

	Sprite* m_pUpperMenu;
	Sprite* m_pLowerMenu;
	Sprite* m_pEndArrow;
	Sprite* m_pEndMenu;
	Sprite* m_pEndPicture;

	ListButton* m_pListButton;	
	Text* m_pEndText;

	void DrawData() const;
	void DrawMenu() const;
	void DrawBackground() const;
	void DrawArrow() const;
};

