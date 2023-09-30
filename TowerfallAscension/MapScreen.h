#pragma once
#include "Screen.h"
#include <vector>
class MapPointer;
class Sprite;
class ResourceManager;
class Text;
class MapScreen final : public Screen
{
public:
	enum class MapState
	{
		IDLE,
		POINTER_TRANSITION
	};
	MapScreen(float windowWidth, float windowHeight, const std::vector<Vector2f>& locations, const std::vector<std::string>& levelNames, const ResourceManager* resources);
	virtual ~MapScreen();

	MapScreen(const MapScreen& c) = delete;
	MapScreen& operator=(const MapScreen& rhs) = delete;
	MapScreen(MapScreen&& c) = delete;
	MapScreen& operator=(MapScreen&& rhs) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	virtual void DrawHud() const override;
	virtual void ProcessKeyPress(const Keyboard* pKeyboard) override;
	virtual int GetButtonValue() const override;
	virtual bool IsKeyPressed() const override;
	virtual Rectf GetMenuBoundaries() const override;
	virtual Point2f GetWidgetPosition() const override;

private:

	int m_NextSelectedButton;
	Sprite* m_pBackground;
	MapState m_MapState;
	Text* m_pText;
	std::vector<std::string> m_LevelNames;
};

