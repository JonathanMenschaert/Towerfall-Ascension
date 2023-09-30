#pragma once
#include "Button.h"
class ButtonStart final : public Button
{
public:
	ButtonStart(const Point2f& location, const Point2f& startLocation, const ResourceManager* resources, float scale = 1.f);
	virtual ~ButtonStart() = default;

	ButtonStart(const ButtonStart& c) = delete;
	ButtonStart& operator=(const ButtonStart& rhs) = delete;
	ButtonStart(ButtonStart&& c) = delete;
	ButtonStart& operator=(ButtonStart&& rhs) = delete;

	virtual void Update(float elapsedSec) override;

private:
	const float m_RotSpeed;
};

