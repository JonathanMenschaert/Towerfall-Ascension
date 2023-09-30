#pragma once

#include "utils.h"
#include "Scenery.h"
#include <vector>
class Sprite;
class AnimatedBackground;
struct LevelInfo;
class ResourceManager;
class Torch;
class Level final
{
public:

	Level(const Window& window, const LevelInfo& levelInfo, ResourceManager* resources);
	~Level();
	Level(const Level& c) = delete;
	Level& operator=(const Level& rhs) = delete;
	Level(Level&& c) = delete;
	Level& operator=(Level&& rhs) = delete;

	void DrawBackground() const;
	void DrawForeground() const;
	void DrawScenery() const;

	void Update(float elapsedSec);
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, bool handleLedges = false) const;
	bool HandleCollision(std::vector<Point2f>& actorShape) const;
	const Point2f& HandleFloatingPath(const Point2f& actorLocation, const Vector2f& actorVelocity, int& currentIdx) const;
	bool CheckHit(const Point2f& p1, const Point2f& p2) const;

	bool IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const;
	bool IsTouchingWall(const Rectf& actorShape, const Vector2f& actorVelocity) const;

	Rectf GetBoundaries() const;

private:

	static const float m_TextureWidth;
	static const float m_TextureHeight;
	static const float m_TextureCameraWidth;

	const Vector2f m_Scale;
	Vector2f m_Translation;

	std::vector<Point2f> m_FloatingPath;
	std::vector<std::vector<Point2f>> m_Vertices;
	std::vector<Torch*> m_pTorches;

	Sprite* m_pBackgroundTexture;
	Sprite* m_pForegroundTexture;

	Rectf m_Boundaries;
	Scenery* m_pScenery;

	ResourceManager* m_pResources;

	void TransformVertices(const std::vector<std::vector<Point2f>>& vertices);
	Scenery* LoadSceneryFromName(Scenery::SceneryType scenery);
};

