#pragma once
#include "Screen.h"
class Sprite;
class ListButton;
class Text;
class ResourceManager;
class PauseScreen :  public Screen
{
public:
	enum class PauseState
	{
		PAUSE,
		PLAY
	};
	enum class ListValue
	{
		RESUME,
		RESTART,
		MAP,
		QUIT,
		//Add above
		ESCAPE
	};
	PauseScreen(float windowWidth, float windowHeight, const ResourceManager* resources);
	virtual ~PauseScreen();
	PauseScreen(const PauseScreen& c) = delete;
	PauseScreen& operator=(const PauseScreen& rhs) = delete;
	PauseScreen(PauseScreen&& c) = delete;
	PauseScreen& operator=(PauseScreen&& rhs) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	virtual void ProcessKeyPress(const Keyboard* pKeyboard) override;
	virtual int GetButtonValue() const override;
	virtual bool IsKeyPressed() const override;

private:
	Sprite* m_pPauseBackground;
	ListButton* m_pListButton;
	PauseState m_PauseState;
	Sprite* m_pPauseArrow;
	Text* m_pPauseText;
};

