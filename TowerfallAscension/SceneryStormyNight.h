#pragma once
#include "Scenery.h"
class Sprite;
class AnimatedBackground;
class ResourceManager;
class SceneryStormyNight final : public Scenery
{
public:
	SceneryStormyNight(const Vector2f& offset, const Vector2f& scale, const Rectf& boundaries, ResourceManager* resources);
	virtual ~SceneryStormyNight();
	SceneryStormyNight(const SceneryStormyNight& c) = delete;
	SceneryStormyNight& operator=(const SceneryStormyNight& rhs) = delete;
	SceneryStormyNight(SceneryStormyNight&& c) = delete;
	SceneryStormyNight& operator=(SceneryStormyNight&& rhs) = delete;


	virtual void Update(float elapsedSec) override;
	virtual void DrawSceneryForeground() const override;
	virtual void DrawSceneryBackground() const override;

private:

	bool m_PlayLightningSound;
	bool m_StrikeLightning;
	float m_ThunderTime;
	const float m_FirstLightning;
	const float m_SecondLightning;
	const float m_LightningDuration;

	Sprite* m_pParallaxTexture;
	Sprite* m_pMoonBackgroundTexture;
	Sprite* m_pMoonTexture;
	AnimatedBackground* m_pRainTexture;
	AnimatedBackground* m_pSmallCloudsTexture;
	AnimatedBackground* m_pLargeCloudsTexture;

	void InitLightningStrike();
	void HandleLightning(float elapsedSec);
};

