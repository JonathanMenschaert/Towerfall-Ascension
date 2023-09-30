#pragma once
#include "utils.h"
class ResourceManager;
class Scenery
{
public:
	enum class SceneryType
	{
		STORMY_NIGHT
	};

	Scenery(const Vector2f& offset, const Vector2f& scale, const Rectf& boundaries, ResourceManager* resources);
	virtual ~Scenery() = default;
	Scenery(const Scenery& c) = delete;
	Scenery& operator=(const Scenery& rhs) = delete;
	Scenery(Scenery&& c) = delete;
	Scenery& operator=(Scenery&& rhs) = delete;


	virtual void Update(float elapsedSec) = 0;
	virtual void DrawSceneryForeground() const = 0;
	virtual void DrawSceneryBackground() const = 0;

protected:
	const Vector2f m_Offset;
	const Vector2f m_Scale;
	const Rectf m_LevelBoundaries;
	ResourceManager* m_pResources;
};

