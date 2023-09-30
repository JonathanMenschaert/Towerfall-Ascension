#pragma once
#include "Sprite.h"
class AnimatedSprite final : public Sprite
{
public:
	enum class AnimationState
	{
		FREEZE,
		LOOP_ONCE,
		LOOP_END,
		LOOP
	};

	AnimatedSprite(const Texture* pSprite, const Rectf& clipRect, int rows, int cols, int framesPerSec, int nrOfFrames, AnimationState animType = AnimationState::FREEZE);
	
	virtual ~AnimatedSprite() = default;
	AnimatedSprite(const AnimatedSprite& sprite) = delete;
	AnimatedSprite& operator=(const AnimatedSprite& sprite) = delete;
	AnimatedSprite(AnimatedSprite&& c) = delete;
	AnimatedSprite& operator=(AnimatedSprite&& rhs) = delete;

	virtual void Draw(const Vector2f& offset = Vector2f{}) const override;
	virtual void Update(float elapsedSec) override;

	void StartAnimation();
	void EndAnimation();
	void NextFrame(int startFrame = -1);

	int GetCurrentFrame() const;
	int GetTotalFrames() const;

private:
	const int m_Rows;
	const int m_Cols;
	const int m_NrOfFrames;
	int m_CurrentFrame;
	const int m_FramesPerSec;
	float m_AnimTime;
	AnimationState m_AnimState;
};

