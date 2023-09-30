#pragma once
#include "Sprite.h"
#include "utils.h"

class Texture;
class AnimatedBackground final : public Sprite
{
public:
	AnimatedBackground(const Texture* pSprite, const Vector2f& movingVector);

	virtual ~AnimatedBackground() = default;
	AnimatedBackground(const AnimatedBackground& sprite) = delete;
	AnimatedBackground& operator=(const AnimatedBackground& sprite) = delete;
	AnimatedBackground(AnimatedBackground&& c) = delete;
	AnimatedBackground& operator=(AnimatedBackground&& rhs) = delete;
	
	virtual void Draw(const Vector2f& offset = Vector2f{}) const override;
	virtual void Update(float elapsedSec) override;
private:
	const Vector2f m_MovingVector;
	Point2f m_TopLeft;
};

