#include "pch.h"
#include "ListButton.h"
#include "Text.h"

ListButton::ListButton(const Point2f& location, const ResourceManager* resources, const std::initializer_list<std::string>& values, float scale)
	:Widget(location, resources, scale)
	,m_SelectedButton{}
	,m_pSelectorArrow{new Text("> ", int(Text::Size::MEDIUM), resources)}
{
	for (const std::string& element : values)
	{
		m_pButtons.push_back(new Text(element, int(Text::Size::MEDIUM), resources));
	}
}

ListButton::~ListButton()
{
	for (size_t i{}; i < m_pButtons.size(); ++i)
	{
		delete m_pButtons[i];
	}
	m_pButtons.clear();
	delete m_pSelectorArrow;
	m_pSelectorArrow = nullptr;
}

void ListButton::Update(float elapsedSec)
{
}

void ListButton::Draw() const
{

	glPushMatrix();
	{
		glTranslatef(m_DefaultPosition.x, m_DefaultPosition.y, 0.f);
		for (size_t i{}; i < m_pButtons.size(); ++i)
		{
			glPushMatrix();
			{
				glTranslatef(-m_pButtons[i]->GetTextWidth() / 2.f * m_Scale, 
					i * -m_pButtons[i]->GetTextHeight() * 1.75f * m_Scale + m_pButtons[i]->GetTextHeight() / 2.f, 0.f);
				glScalef(m_Scale, m_Scale, 1.f);
				m_pButtons[i]->Draw();
				if (m_SelectedButton == i)
				{
					glTranslatef(-m_pSelectorArrow->GetTextWidth(), 0.f, 0.f);
					m_pSelectorArrow->Draw();
				}
			}
			glPopMatrix();
		}
	}
	glPopMatrix();
}

void ListButton::Select(int select)
{
	m_SelectedButton += select;
	if (m_SelectedButton < 0)
	{
		m_SelectedButton = int(m_pButtons.size()) - 1;
	}
	else
	{
		m_SelectedButton = m_SelectedButton % m_pButtons.size();
	}
}

int ListButton::GetButtonValue() const
{
	return m_SelectedButton;
}
