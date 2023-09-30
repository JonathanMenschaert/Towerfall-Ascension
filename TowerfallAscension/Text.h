#pragma once
#include <vector>

class Texture;
class ResourceManager;
class Text final
{
public:
	enum class Size
	{
		SMALL,
		MEDIUM,
		LARGE,

		AMOUNT
	};
	Text(const std::string& text, const int fontSizeIdx, const ResourceManager* resources);
	~Text();

	Text(const Text& c) = delete;
	Text& operator=(const Text& rhs) = delete;
	Text(Text&& c) = delete;
	Text& operator=(Text&& rhs) = delete;

	void Draw() const;
	void SetNewText(const std::string& text, int fontSize);
	float GetTextWidth() const;
	float GetTextHeight() const;
private:

	const ResourceManager* m_pResources;
	std::vector<Texture*> m_pText;
	std::string m_String;
};

