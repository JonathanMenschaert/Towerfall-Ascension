#pragma once
#include "Button.h"
class ResourceManager;
class OptionButton final : public Button
{
public:
	OptionButton(const Point2f& location, const Point2f& startLocation, const ResourceManager* resources, float scale = 1.f);
	virtual ~OptionButton() = default;

	OptionButton(const OptionButton& c) = delete;
	OptionButton& operator=(const OptionButton& rhs) = delete;
	OptionButton(OptionButton&& c) = delete;
	OptionButton& operator=(OptionButton&& rhs) = delete;

	virtual void Update(float elapsedSec) override;

private:
	const float m_RotSpeed;
};

