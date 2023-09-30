#include "pch.h"
#include "Text.h"
#include "Texture.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include <string>

Text::Text(const std::string& text, const int fontSizeIdx, const ResourceManager* resources)
	:m_String{text}
	,m_pResources{resources}
{
	TextureManager* textureManager = resources->GetTextureManager();
	for (size_t i{}; i < m_String.size(); ++i)
	{
		char c{ char(std::toupper(m_String[i])) };		
		m_pText.push_back(textureManager->GetCharacter(c, fontSizeIdx));
	}
}

Text::~Text()
{
}

float Text::GetTextWidth() const
{
	float width{};
	for (size_t i{}; i < m_pText.size(); ++i)
	{
		width += m_pText[i]->GetWidth();
	}
	return width;
}

float Text::GetTextHeight() const
{
	return m_pText[0]->GetHeight();
}


void Text::Draw() const
{
	glPushMatrix();
	{
		for (size_t i{}; i < m_pText.size(); ++i)
		{
			m_pText[i]->Draw();
			glTranslatef(m_pText[i]->GetWidth(), 0.f, 0.f);
		}
	}
	glPopMatrix();
}

void Text::SetNewText(const std::string& text, int fontSize)
{
	m_pText.clear();
	m_String = text;
	for (size_t i{}; i < m_String.size(); ++i)
	{
		char c{ char(std::toupper(m_String[i])) };
		m_pText.push_back(m_pResources->GetTextureManager()->GetCharacter(c, fontSize));
	}
}
