#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include <SDL_opengl.h>
#include <iostream>
#include "SVGParser.h"
#include "Sprite.h"
#include "AnimatedBackground.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "Matrix2x3.h"
#include "Arrow.h"
#include "SceneryStormyNight.h"
#include "LevelTemplate.h"
#include "Torch.h"
#include "SOundManager.h"

const float Level::m_TextureWidth{ 1920.f };
const float Level::m_TextureHeight{ 1080.f };
const float Level::m_TextureCameraWidth{ 1440.f };

Level::Level(const Window& window, const LevelInfo& levelInfo, ResourceManager* resources)
	: m_Scale{ window.width / m_TextureWidth, window.height / m_TextureHeight} 
	,m_pResources{resources}
{
	m_Translation = Vector2f{ (m_TextureWidth - m_TextureCameraWidth) / 2.f * m_Scale.x, 0.f };
	
	TextureManager* textureManager = m_pResources->GetTextureManager();
	m_pBackgroundTexture = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::SACRED_GROUND_BACKGROUND));
	m_pBackgroundTexture->SetScale(m_Scale);

	m_pForegroundTexture = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::SACRED_GROUND_FOREGROUND));
	m_pForegroundTexture->SetScale(m_Scale);

	m_Boundaries = Rectf{ m_Translation.x, m_Translation.y, m_Scale.x * m_TextureCameraWidth, m_Scale.y * m_TextureHeight };

	TransformVertices(levelInfo.vertices);
	m_pScenery = LoadSceneryFromName(levelInfo.sceneryType);
	m_FloatingPath = levelInfo.floatingPath;

	for (size_t i{}; i < levelInfo.torchLocations.size(); ++i)
	{
		m_pTorches.push_back(new Torch(levelInfo.torchLocations[i], resources));
	}

	resources->GetSoundManager()->StopTrack();
	resources->GetSoundManager()->PlayTrack(levelInfo.music);

}

Level::~Level()
{
	delete m_pBackgroundTexture;
	m_pBackgroundTexture = nullptr;

	delete m_pForegroundTexture;
	m_pForegroundTexture = nullptr;

	delete m_pScenery;
	m_pScenery = nullptr;


	for (size_t i{}; i < m_pTorches.size(); ++i)
	{
		delete m_pTorches[i];
	}
	m_pTorches.clear();
}

void Level::Update(float elapsedSec)
{
	m_pScenery->Update(elapsedSec);
	for (size_t i{}; i < m_pTorches.size(); ++i)
	{
		m_pTorches[i]->Update(elapsedSec);
	}
}

void Level::DrawBackground() const
{
	glPushMatrix();
	{
		glTranslatef(m_Translation.x, m_Translation.y, 0.f);
		m_pScenery->DrawSceneryForeground();
		m_pBackgroundTexture->Draw();		
	}
	glPopMatrix();
	for (size_t i{}; i < m_pTorches.size(); ++i)
	{
		m_pTorches[i]->Draw();
	}
}

void Level::DrawForeground() const
{
	glPushMatrix();
	{
		glTranslatef(m_Translation.x, m_Translation.y, 0.f);
		m_pForegroundTexture->Draw();
	}
	glPopMatrix();
}

void Level::DrawScenery() const
{
	m_pScenery->DrawSceneryBackground();
}

bool Level::HandleCollision(std::vector<Point2f>& actorShape) const
{
	utils::HitInfo hitInfo{};

	for (unsigned int i{}; i < actorShape.size() - 1; ++i)
	{
		Point2f p1{ actorShape[i] };
		Point2f p2{ actorShape[i + size_t(1)] };
		
		for (unsigned int j{}; j < m_Vertices.size(); ++j)
		{
			if (utils::Raycast(m_Vertices[j], p1, p2, hitInfo))
			{
				return true;
			}
		}
	}
	return false;
}

const Point2f& Level::HandleFloatingPath(const Point2f& actorLocation, const Vector2f& actorVelocity, int& currentIdx) const
{
	float minDistanceSq{ FLT_MAX };
	int minIdx{ -1 };
	const float offset{ 5.f };
	for (size_t i{}; i < m_FloatingPath.size(); ++i)
	{
		Point2f destination{ m_FloatingPath[i] };
		if (!CheckHit(destination, actorLocation))
		{
			
			float deltaDistanceX{ destination.x - actorLocation.x };
			float deltaDistanceY{ destination.y - actorLocation.y };

			if (deltaDistanceX < -offset && actorVelocity.x > 0.f)
			{
				destination.x += -m_Boundaries.width;
			}
			else if (deltaDistanceX > offset && actorVelocity.x < 0.f)
			{
				destination.x += m_Boundaries.width;
			}

			if (deltaDistanceY < -offset && actorVelocity.y > 0.f)
			{
				destination.y += -m_Boundaries.height;
			}
			else if (deltaDistanceY > offset && actorVelocity.y < 0.f)
			{
				destination.y += m_Boundaries.height;
			}
			

			Vector2f distanceVector{ destination - actorLocation };
			float distanceSq{ distanceVector.SquaredLength() };
			if (distanceSq < minDistanceSq && currentIdx != int(i))
			{
				minDistanceSq = distanceSq;
				minIdx = int(i);
			}
		}
	}

	currentIdx = minIdx;
	return m_FloatingPath[currentIdx];
}


void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, bool handleLedges) const
{
	const float highModifier{ 0.9375f }, middleModifier{ 0.5f }, lowModifier{ 0.0625 };

	Point2f vertRight1{ actorShape.left + highModifier * actorShape.width, actorShape.bottom };
	Point2f vertRight2{ vertRight1.x, vertRight1.y + actorShape.height};

	Point2f vertLeft1{ actorShape.left + lowModifier * actorShape.width, actorShape.bottom };
	Point2f vertLeft2{ vertLeft1.x, vertLeft1.y + actorShape.height};

	Point2f horMiddle1{ actorShape.left, actorShape.bottom + actorShape.height * middleModifier };
	Point2f horMiddle2{ horMiddle1.x + actorShape.width, horMiddle1.y };

	Point2f horBottom1{ actorShape.left, actorShape.bottom + lowModifier * actorShape.height };
	Point2f horBottom2{ horBottom1.x + actorShape.width, horBottom1.y };

	Point2f horTop1{ actorShape.left, actorShape.bottom + highModifier * actorShape.height };
	Point2f horTop2{ horTop1.x + actorShape.width, horTop1.y };

	utils::HitInfo hitInfo{};

	for (unsigned int i{}; i < m_Vertices.size(); ++i)
	{
		bool hitTop{ utils::Raycast(m_Vertices[i], horTop1, horTop2, hitInfo) };
		bool hitMiddle{ utils::Raycast(m_Vertices[i], horMiddle1, horMiddle2, hitInfo) };
		bool hitBottom{ utils::Raycast(m_Vertices[i], horBottom1, horBottom2, hitInfo) };

		if ((hitTop || hitMiddle || hitBottom) && actorVelocity.x != 0.f)
		{
			float difference{ actorShape.left - hitInfo.intersectPoint.x };
			if (abs(difference) < abs(difference + actorShape.width))
			{
				actorShape.left = hitInfo.intersectPoint.x;
			}
			else
			{
				actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
			}
		}
		if (handleLedges)
		{
			if (hitMiddle && hitBottom)
			{
				if (hitTop)
				{
					actorVelocity.y = -200.f;
				}
				else
				{
					actorVelocity.y = 0.f;
				}
			}
		}
		if (utils::Raycast(m_Vertices[i], vertRight1, vertRight2, hitInfo) 
			|| utils::Raycast(m_Vertices[i], vertLeft1, vertLeft2, hitInfo))
		{
			float difference{actorShape.bottom - hitInfo.intersectPoint.y };
			if (abs(difference) < abs(difference + actorShape.height))
			{
				actorShape.bottom = hitInfo.intersectPoint.y;
				actorVelocity.y = 0.f;
			}
			else if (actorVelocity.y > 0.f)
			{
				actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height;
				actorVelocity.y = -0.001f;
			}			
		}		
	}
}

bool Level::IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const
{
	const float highModifier{ 0.9375f }, lowModifier{ 0.0625 };
	const float depth{ 1.f };

	Point2f vertRight1{ actorShape.left + highModifier * actorShape.width, actorShape.bottom - depth};
	Point2f vertRight2{ vertRight1.x, actorShape.bottom + actorShape.height };	

	Point2f vertLeft1{ actorShape.left + lowModifier * actorShape.width, actorShape.bottom - depth};
	Point2f vertLeft2{ vertLeft1.x, actorShape.bottom + actorShape.height };

	utils::HitInfo hitInfo{};

	bool isOnGround{ false };

	for (unsigned int i{}; i < m_Vertices.size(); ++i)
	{
		isOnGround |= utils::Raycast(m_Vertices[i], vertRight1, vertRight2, hitInfo)
			|| utils::Raycast(m_Vertices[i], vertLeft1, vertLeft2, hitInfo);
	}

	return isOnGround;
}

bool Level::IsTouchingWall(const Rectf& actorShape, const Vector2f& actorVelocity) const
{
	const float highModifier{ 0.9375f }, middleModifier{ 0.5f }, lowModifier{ 0.0625 };
	const float depth{ 1.f };
	Point2f horMiddle1{ actorShape.left - depth, actorShape.bottom + actorShape.height * middleModifier };
	Point2f horMiddle2{ actorShape.left + actorShape.width + depth, horMiddle1.y };

	Point2f horBottom1{ actorShape.left - depth, actorShape.bottom + lowModifier * actorShape.height };
	Point2f horBottom2{ actorShape.left + actorShape.width + depth, horBottom1.y };

	Point2f horTop1{ actorShape.left - depth, actorShape.bottom + highModifier * actorShape.height };
	Point2f horTop2{ actorShape.left + actorShape.width + depth, horTop1.y };

	utils::HitInfo hitInfo{};
	bool isTouchingWall{ false };

	for (unsigned int i{}; i < m_Vertices.size(); ++i)
	{
		isTouchingWall = isTouchingWall || (utils::Raycast(m_Vertices[i], horMiddle1, horMiddle2, hitInfo)
			|| utils::Raycast(m_Vertices[i], horBottom1, horBottom2, hitInfo)
			|| utils::Raycast(m_Vertices[i], horTop1, horTop2, hitInfo));
	}

	return isTouchingWall;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}


void Level::TransformVertices(const std::vector<std::vector<Point2f>>& vertices)
{
	Matrix2x3 matTranslate, matScale;
	matTranslate.SetAsTranslate(m_Translation);
	matScale = Matrix2x3::CreateScalingMatrix(m_Scale.x, m_Scale.y);

	Matrix2x3 matWorld{ matTranslate * matScale };
	for (unsigned int i{}; i < vertices.size(); ++i)
	{
		m_Vertices.push_back(matWorld.Transform(vertices[i]));
	}
}

Scenery* Level::LoadSceneryFromName(Scenery::SceneryType scenery)
{
	switch (scenery)
	{
	case Scenery::SceneryType::STORMY_NIGHT:
		return new SceneryStormyNight(m_Translation, m_Scale, m_Boundaries, m_pResources);
	default:
		return nullptr;
	}
}

bool Level::CheckHit(const Point2f& p1, const Point2f& p2) const
{
	utils::HitInfo hitInfo{};
	for (unsigned int i{}; i < m_Vertices.size(); ++i)
	{
		if (utils::Raycast(m_Vertices[i], p1, p2, hitInfo))
		{
			return true;
		}
	}
	return false;
}
