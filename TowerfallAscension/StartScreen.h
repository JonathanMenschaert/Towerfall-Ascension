#pragma once
#include "Screen.h"
#include <vector>
class Sprite;
class AnimatedSprite;
class Text;
class Widget;

class StartScreen final : public Screen
{
public:
	enum class ScreenState
	{
		TITLE,
		TRANSITION_MENU,
		MENU,
		TRANSITION_TITLE
	};
	StartScreen(float windowWidth, float windowHeight, const ResourceManager* resources);
	virtual ~StartScreen();

	StartScreen(const StartScreen& c) = delete;
	StartScreen& operator=(const StartScreen& rhs) = delete;
	StartScreen(StartScreen&& c) = delete;
	StartScreen& operator=(StartScreen&& rhs) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	virtual void ProcessKeyPress(const Keyboard* pKeyboard) override;
	virtual int GetButtonValue() const override;
	virtual bool IsKeyPressed() const override;

private:
	bool m_ShouldTextShow;
	float m_JitterInterval;
	float m_JitterTime;
	float m_LetterAngle;
	const float m_LetterRotationSpeed;
	const float m_LetterRotationInterval;
	const float m_MaxJitterTime;
	const float m_MaxJitterInterval;

	Text* m_pText;

	Sprite* m_pTower;	
	Sprite* m_pOrbBottom;
	Sprite* m_pBanner;
	Sprite* m_pArrow;
	Sprite* m_pTitle;
	std::vector<Sprite*> m_pLetters;

	AnimatedSprite* m_pOrb;
	AnimatedSprite* m_pBackground;
	

	ScreenState m_State;

	void DrawBackground() const;
	void DrawOrb() const;
	void DrawTower() const;
	void DrawBanner() const;
	void DrawTitle() const;
	void DrawArrow() const;
	void DrawLetters() const;
	void DrawMenuText() const;
	void DrawButtons() const;
	
};

