#pragma once
#include "Widget.h"
#include <initializer_list>
#include <vector>
class Text;
class ListButton final :  public Widget
{
public:
	ListButton(const Point2f& location, const ResourceManager* resources, const std::initializer_list<std::string>& values, float scale = 1.f);
	virtual ~ListButton();
	ListButton(const ListButton& c) = delete;
	ListButton& operator=(const ListButton& rhs) = delete;
	ListButton(ListButton&& c) = delete;
	ListButton& operator=(ListButton&& rhs) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	virtual void Select(int select) override;
	virtual int GetButtonValue() const override;


private:
	int m_SelectedButton;
	std::vector<Text*> m_pButtons;
	Text* m_pSelectorArrow;
};

