#pragma once
#include "Screen.h"
class Sprite;
class AnimatedSprite;
class ResourceManager;
class Keyboard;
class Text;
class OptionsScreen final : public Screen
{
public:

	enum class Option
	{
		SOUND_SFX,
		SOUND_MUSIC
	};
	OptionsScreen(float windowWidth, float windowHeight, const ResourceManager* resources);
	virtual ~OptionsScreen();

	OptionsScreen(const OptionsScreen& c) = delete;
	OptionsScreen& operator=(const OptionsScreen& rhs) = delete;
	OptionsScreen(OptionsScreen&& c) = delete;
	OptionsScreen& operator=(OptionsScreen&& rhs) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	virtual void ProcessKeyPress(const Keyboard* pKeyboard) override;
	virtual bool IsKeyPressed() const override;
	virtual int GetButtonValue() const override;

private:
	Sprite* m_pTower;
	AnimatedSprite* m_pBackground;
	Text* m_pTitle;

	void ChangeOption(Option option);
	void DrawTower() const;
	void DrawTitle() const;
	void DrawButtons() const;
};

