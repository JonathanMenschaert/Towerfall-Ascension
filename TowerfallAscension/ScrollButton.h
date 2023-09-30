#pragma once
#include "Widget.h"
class ResourceManager;
class Sprite;
class Text;
class ScrollButton final : public Widget
{
public:
	ScrollButton(const Point2f& location, const ResourceManager* resources, const std::string& text, int min, int max, int current, float scale = 1.f);
	virtual ~ScrollButton();
	ScrollButton(const ScrollButton& c) = delete;
	ScrollButton& operator=(const ScrollButton& rhs) = delete;
	ScrollButton(ScrollButton&& c) = delete;
	ScrollButton& operator=(ScrollButton&& rhs) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	virtual void Select(int select) override;
	virtual int GetButtonValue() const override;
	virtual void AddButtonValue(int value) override;

private:


	int m_CurrentValue;
	const int m_MinValue, m_MaxValue;
	float m_ArrowAngle;
	const float m_ArrowRotSpeed;

	const float m_OpenSpace;

	Sprite* m_pScrollButton;
	Text* m_pValue;
	Text* m_pName;

	void DrawArrow(int dir) const;
	void DrawName() const;
};

