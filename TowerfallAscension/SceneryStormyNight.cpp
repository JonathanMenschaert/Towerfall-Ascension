#include "pch.h"
#include "SceneryStormyNight.h"
#include "Sprite.h"
#include "AnimatedBackground.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "SoundManager.h"

SceneryStormyNight::SceneryStormyNight(const Vector2f& offset, const Vector2f& scale, const Rectf& boundaries, ResourceManager* resources)
	:Scenery(offset, scale, boundaries, resources)
	, m_StrikeLightning{ false }
	, m_ThunderTime{}
	, m_FirstLightning{ 1.2f }
	, m_SecondLightning{ 0.4f }
	, m_LightningDuration{ 0.4f }
	, m_PlayLightningSound{true}
{
	TextureManager* textureManager = resources->GetTextureManager();
	m_pParallaxTexture = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::SCENERY_BACKGROUND));
	m_pParallaxTexture->SetScale(m_Scale);

	Texture* pMoonBackground{ textureManager->GetTexture(TextureManager::TextureSheet::SCENERY_MOONBACKGROUND) };
	float scaleModifier{ 4.5f };
	m_pMoonBackgroundTexture = new Sprite(pMoonBackground,
		Rectf{ pMoonBackground->GetWidth() * 0.6f, pMoonBackground->GetHeight(), pMoonBackground->GetWidth(), pMoonBackground->GetHeight() });
	m_pMoonBackgroundTexture->SetScale(m_Scale * scaleModifier);

	m_pMoonTexture = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::SCENERY_MOON));
	m_pMoonTexture->SetScale(Vector2f{ m_Scale.x * scaleModifier, m_Scale.x * scaleModifier });

	m_pRainTexture = new AnimatedBackground(textureManager->GetTexture(TextureManager::TextureSheet::SCENERY_RAIN),
		Vector2f{ -70.f, -70.f });
	m_pRainTexture->SetScale(m_Scale * scaleModifier);

	m_pSmallCloudsTexture = new AnimatedBackground(textureManager->GetTexture(TextureManager::TextureSheet::SCENERY_SMALLCLOUDS),
		Vector2f{ -40.f, 0.f });
	m_pSmallCloudsTexture->SetScale(m_Scale * scaleModifier);

	m_pLargeCloudsTexture = new AnimatedBackground(textureManager->GetTexture(TextureManager::TextureSheet::SCENERY_LARGECLOUDS),
		Vector2f{ -10.f, 0.f });
	m_pLargeCloudsTexture->SetScale(m_Scale * scaleModifier);

	InitLightningStrike();
}

SceneryStormyNight::~SceneryStormyNight()
{
	delete m_pParallaxTexture;
	m_pParallaxTexture = nullptr;

	delete m_pMoonBackgroundTexture;
	m_pMoonBackgroundTexture = nullptr;

	delete m_pMoonTexture;
	m_pMoonTexture = nullptr;

	delete m_pRainTexture;
	m_pRainTexture = nullptr;

	delete m_pSmallCloudsTexture;
	m_pSmallCloudsTexture = nullptr;

	delete m_pLargeCloudsTexture;
	m_pLargeCloudsTexture = nullptr;
}

void SceneryStormyNight::Update(float elapsedSec)
{
	m_pRainTexture->Update(elapsedSec);
	m_pSmallCloudsTexture->Update(elapsedSec);
	m_pLargeCloudsTexture->Update(elapsedSec);
	HandleLightning(elapsedSec);
}

void SceneryStormyNight::DrawSceneryForeground() const
{
	m_pRainTexture->Draw();
}

void SceneryStormyNight::DrawSceneryBackground() const
{
	glPushMatrix();
	{
		glTranslatef(m_Offset.x, m_Offset.y, 0.f);
		m_pParallaxTexture->Draw();
 		m_pMoonBackgroundTexture->Draw();
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(m_Offset.x + m_LevelBoundaries.width * 0.66f, m_Offset.y + m_LevelBoundaries.height * 0.67f, 0.f);
		m_pMoonTexture->Draw();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(m_Offset.x, m_Offset.y, 0.f);
		m_pLargeCloudsTexture->Draw();
		m_pSmallCloudsTexture->Draw();
		if (m_StrikeLightning)
		{
			utils::SetColor(Color4f{ 158.f / 255.f, 218.f / 255.f, 240.f / 255.f, 1.f });
			utils::FillRect(0.f, 0.f, m_LevelBoundaries.width, m_LevelBoundaries.height);
		}
	}
	glPopMatrix();
}

void SceneryStormyNight::HandleLightning(float elapsedSec)
{
	m_ThunderTime -= elapsedSec;
	m_StrikeLightning = false;
	if (m_ThunderTime <= m_FirstLightning && m_ThunderTime >= m_FirstLightning - m_LightningDuration)
	{
		m_StrikeLightning = true;
		if (m_PlayLightningSound)
		{
			int random{ rand() % 3 };
			int lightningSound{ int(SoundManager::SoundEffectType::LIGHTNING_1) + random };
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType(lightningSound));
			m_PlayLightningSound = false;
		}
	}
	else if (m_ThunderTime <= m_SecondLightning && m_ThunderTime >= m_SecondLightning - m_LightningDuration)
	{
		m_StrikeLightning = true;
	}
	else if (m_ThunderTime < m_SecondLightning - m_LightningDuration)
	{
		InitLightningStrike();
	}
}

void SceneryStormyNight::InitLightningStrike()
{
	m_PlayLightningSound = true;
	const int m_MaxThunderTime{ 20 };
	const int m_MinThunderTime{ 10 };
	m_ThunderTime = float(rand() % int(m_MaxThunderTime - m_MinThunderTime)) + m_MinThunderTime;
}

