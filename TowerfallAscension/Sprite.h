#pragma once
#include "utils.h"
class Texture;

class Sprite
{
public:	
	Sprite(const Texture* pSprite, const Rectf& clipRect);

	Sprite(const Texture* pSprite);

	virtual ~Sprite() = default;
	Sprite(const Sprite& sprite) = delete;
	Sprite& operator=(const Sprite& sprite) = delete;
	Sprite(Sprite&& c) = delete;
	Sprite& operator=(Sprite&& rhs) = delete;

	virtual void Draw(const Vector2f& offset = Vector2f{}) const;
	virtual void Update(float elapsedSec);
	void SetScale(const Vector2f& scale);
	void SetAngle(float angle);
	float GetHeight() const;
	float GetWidth() const;
	float GetScaledWidth() const;
	float GetScaledHeight() const;

	void SetClipPosition(const Point2f& position);
	
protected:
	const Texture* m_pSprite;
	Rectf m_ClipRect;
	Vector2f m_Scale;
	float m_Angle;	
};

