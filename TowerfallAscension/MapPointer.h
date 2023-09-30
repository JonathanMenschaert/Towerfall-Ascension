#pragma once
#include "Widget.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "MapPointer.h"

class Sprite;
class MapPointer final : public Widget
{
public:
	enum class PointerState
	{
		HOVERING,
		MOVING
	};
	MapPointer(const Point2f& location, const ResourceManager* resources, float scale = 1.f, int value= int(Screen::ScreenType::NONE), bool isClickable = true);
	virtual ~MapPointer() ;
	MapPointer(const MapPointer& c) = delete;
	MapPointer& operator=(const MapPointer& rhs) = delete;
	MapPointer(MapPointer&& c) = delete;
	MapPointer& operator=(MapPointer&& rhs) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	virtual void Select(bool select) override;
	virtual bool IsSelected() const override;
	virtual void SetPressed(bool pressed) override;
	virtual void SetMovePosition(const Point2f& newLocation) override;
	virtual bool IsMoving() const override;
private:
	float m_MaxMoveDistance;
	float m_BobbingAngle;
	const float m_MoveSpeed;
	const float m_BobbingSpeed;
	const float m_MaxBobbingAngle;
	const float m_BobbingRadius;
	
	Vector2f m_MoveVector;
	PointerState m_PointerState;
	Sprite* m_pPointer;
};

