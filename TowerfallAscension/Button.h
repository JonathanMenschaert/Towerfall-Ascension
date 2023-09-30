#pragma once
#include "Widget.h"
#include "utils.h"
#include "Text.h"
class Texture;
class Sprite;


class Button : public Widget
{
public:
	Button(Texture* pTexture, const Point2f& location, const Point2f& startLocation, const std::string& buttonText, const Text::Size fontSize,
		Screen::ScreenType value, const ResourceManager* resources, const Vector2f& textOffset = Vector2f{ 0.f, 0.f }, float scale = 1.f);
	virtual ~Button();

	Button(const Button& c) = delete;
	Button& operator=(const Button& rhs) = delete;
	Button(Button&& c) = delete;
	Button& operator=(Button&& rhs) = delete;

	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const override;

	virtual void Select(bool select) override;
	virtual void SetActive(bool active) override;
	virtual bool IsSelected() const override;

protected:
	enum class ButtonState
	{
		DEACTIVATED,
		ON_ACTIVATION,
		ON_SELECTION,
		SELECTED,
		ON_DESELECTION,
		DESELECTED,
		ON_DEACTIVATION
	};

	float m_TextScale;
	float m_Rotation;
	float m_BobbingAngle;
	
	const float m_BobbingSpeed;
	const float m_BobbingRadius;
	const float m_MaxRotation;
	const float m_ScaleSpeed;
	const float m_MaxTextScale;
	const float m_MinTextScale;
	const float m_ActivationSpeed;
	const float m_ActivationDistance;

	Sprite* m_pButtonTexture;
	Text* m_pButtonText;
	ButtonState m_ButtonState;

	Vector2f m_MoveDirection;
	Vector2f m_TextOffset;
};

