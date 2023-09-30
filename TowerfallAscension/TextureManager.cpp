#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"
#include <string>
#include <iostream>
#include "Text.h"

TextureManager::TextureManager()
{
	LoadTextures();
}

TextureManager::~TextureManager()
{
	for (size_t i{}; i < m_pGameTextures.size(); ++i)
	{
		delete m_pGameTextures[TextureSheet(i)];
	}
	m_pGameTextures.clear();

	for (size_t i{}; i < m_pGameFont.size(); ++i)
	{
		for (size_t j{}; j < 127; ++j)
		{
			delete m_pGameFont[i][char(j)];
		}
	}
	m_pGameFont.clear();
}

void TextureManager::LoadTextures()
{
	LoadGameTextures();
	LoadGameFont();
}

void TextureManager::LoadGameFont()
{
	const std::string fontPath{ "Resources/Font/archer.ttf" };
	const int fontSizeAmount{ int(Text::Size::AMOUNT) };
	const int fontSizes[fontSizeAmount]{20, 40, 60};
	const std::vector<char> specialChars {' ', ',', ':', ';', '.', '?', '!', '>'};
	for (size_t i{}; i < fontSizeAmount; ++i)
	{
		std::unordered_map<char, Texture*> pGameFont;
		for (char j{ 'A'}; j <= 'Z'; ++j)
		{
			const std::string ascii{ char(j) };
			pGameFont[j] = new Texture(ascii, fontPath, fontSizes[i], Color4f{1.f, 1.f, 1.f, 1.f});
		}
		for (char j{ '0' }; j <= '9'; ++j)
		{
			const std::string ascii{ char(j) };
			pGameFont[j] = new Texture(ascii, fontPath, fontSizes[i], Color4f{ 1.f, 1.f, 1.f, 1.f });
		}

		for (size_t j{}; j < specialChars.size(); ++j)
		{
			const std::string ascii{ specialChars[j] };
			pGameFont[specialChars[j]] = new Texture(ascii, fontPath, fontSizes[i], Color4f{ 1.f, 1.f, 1.f, 1.f });
		}

		m_pGameFont.push_back(pGameFont);
	}	
}

void TextureManager::LoadGameTextures()
{
	m_pGameTextures[TextureSheet::PLAYER_BODY] = new Texture("Resources/Images/Player/PlayerBody.png");
	m_pGameTextures[TextureSheet::PLAYER_HEADS] = new Texture("Resources/Images/Player/PlayerHeads.png");
	m_pGameTextures[TextureSheet::PLAYER_ROLL] = new Texture("Resources/Images/Player/PlayerRoll.png");
	m_pGameTextures[TextureSheet::PLAYER_BOW] = new Texture("Resources/Images/Player/PlayerBow.png");
	m_pGameTextures[TextureSheet::PLAYER_RESPAWN] = new Texture("Resources/Images/Player/PlayerRespawn.png");
	m_pGameTextures[TextureSheet::PLAYER_EXPLOSION] = new Texture("Resources/Images/Player/PlayerExplosion.png");
	m_pGameTextures[TextureSheet::ARROW_AIM] = new Texture("Resources/Images/Player/ArrowAim.png");
	m_pGameTextures[TextureSheet::ARROW_PLAYER] = new Texture("Resources/Images/Player/ArrowPlayer.png");
	m_pGameTextures[TextureSheet::ARROW_QUIVER] = new Texture("Resources/Images/Hud/ArrowQuiver.png");
	m_pGameTextures[TextureSheet::SPAWN_PORTAL] = new Texture("Resources/Images/Enemy/SpawnPortal.png");
	m_pGameTextures[TextureSheet::GHOST_WANDERING] = new Texture("Resources/Images/Enemy/GhostWandering.png");
	m_pGameTextures[TextureSheet::GHOST_DODGE] = new Texture("Resources/Images/Enemy/GhostDodge.png");
	m_pGameTextures[TextureSheet::GHOST_DYING] = new Texture("Resources/Images/Enemy/GhostDying.png");
	m_pGameTextures[TextureSheet::GHOST_CONFUSION] = new Texture("Resources/Images/Enemy/GhostConfusion.png");
	m_pGameTextures[TextureSheet::SLIME_MOVEMENT] = new Texture("Resources/Images/Enemy/Slime.png");
	m_pGameTextures[TextureSheet::LEVEL_TORCH] = new Texture("Resources/Images/Level/LevelTorch.png");
	m_pGameTextures[TextureSheet::SACRED_GROUND_FOREGROUND] = new Texture("Resources/Images/Level/SacredGroundForeground.png");
	m_pGameTextures[TextureSheet::SACRED_GROUND_BACKGROUND] = new Texture("Resources/Images/Level/SacredGroundBackground.png");
	m_pGameTextures[TextureSheet::WAVE_NUMBER] = new Texture("Resources/Images/Level/WaveNumber.png");
	m_pGameTextures[TextureSheet::SCENERY_BACKGROUND] = new Texture("Resources/Images/Scenery/SceneryBackground.png");
	m_pGameTextures[TextureSheet::HUD_SIDE] = new Texture("Resources/Images/Hud/WindowSide.png");
	m_pGameTextures[TextureSheet::HUD_LIVES] = new Texture("Resources/Images/Hud/HudLives.png");
	m_pGameTextures[TextureSheet::SCENERY_MOONBACKGROUND] = new Texture("Resources/Images/Scenery/SceneryMoonBackground.png");
	m_pGameTextures[TextureSheet::SCENERY_MOON] = new Texture("Resources/Images/Scenery/SceneryMoon.png");
	m_pGameTextures[TextureSheet::SCENERY_SMALLCLOUDS] = new Texture("Resources/Images/Scenery/ScenerySmallClouds.png");
	m_pGameTextures[TextureSheet::SCENERY_LARGECLOUDS] = new Texture("Resources/Images/Scenery/SceneryLargeClouds.png");
	m_pGameTextures[TextureSheet::SCENERY_RAIN] = new Texture("Resources/Images/Scenery/SceneryRain.png");
	m_pGameTextures[TextureSheet::UPGRADE_CHEST] = new Texture("Resources/Images/Misc/UpgradeChest.png");
	m_pGameTextures[TextureSheet::UPGRADE_SHIELD] = new Texture("Resources/Images/Misc/Shield.png");
	m_pGameTextures[TextureSheet::UPGRADE_SHIELD_AURA] = new Texture("Resources/Images/Misc/ShieldAura.png");
	m_pGameTextures[TextureSheet::UPGRADE_ARROW] = new Texture("Resources/Images/Misc/UpgradeArrow.png");
	m_pGameTextures[TextureSheet::UPGRADE_HAT] = new Texture("Resources/Images/Misc/UpgradeHat.png");
	m_pGameTextures[TextureSheet::START_BACKGROUND] = new Texture("Resources/Images/Menu/StartBackground.png");
	m_pGameTextures[TextureSheet::START_BANNER] = new Texture("Resources/Images/Menu/StartBanner.png");
	m_pGameTextures[TextureSheet::START_ARROW] = new Texture("Resources/Images/Menu/StartArrow.png");
	m_pGameTextures[TextureSheet::START_ORB] = new Texture("Resources/Images/Menu/StartOrb.png");
	m_pGameTextures[TextureSheet::START_ORBBOTTOM] = new Texture("Resources/Images/Menu/StartOrbBottom.png");
	m_pGameTextures[TextureSheet::START_TITLE] = new Texture("Resources/Images/Menu/StartTitle.png");
	m_pGameTextures[TextureSheet::START_TOWER] = new Texture("Resources/Images/Menu/StartTower.png");
	m_pGameTextures[TextureSheet::START_LETTERS] = new Texture("Resources/Images/Menu/StartLetters.png");
	m_pGameTextures[TextureSheet::OPTIONS_TOWER] = new Texture("Resources/Images/Menu/OptionsTower.png");
	m_pGameTextures[TextureSheet::MAP_BACKGROUND] = new Texture("Resources/Images/Menu/MapBackground.png");
	m_pGameTextures[TextureSheet::MAP_POINTER] = new Texture("Resources/Images/Menu/MapPointer.png");
	m_pGameTextures[TextureSheet::PAUSE_BACKGROUND] = new Texture("Resources/Images/Menu/PauseBackground.png");
	m_pGameTextures[TextureSheet::PAUSE_ARROW] = new Texture("Resources/Images/Menu/PauseArrow.png");
	m_pGameTextures[TextureSheet::END_BACKGROUND] = new Texture("Resources/Images/Menu/EndBackground.png");
	m_pGameTextures[TextureSheet::END_PICTURE] = new Texture("Resources/Images/Menu/EndPicture.png");
	m_pGameTextures[TextureSheet::BUTTON_START] = new Texture("Resources/Images/Menu/ButtonStart.png");	
	m_pGameTextures[TextureSheet::BUTTON_SCROLL] = new Texture("Resources/Images/Menu/ScrollButtonArrow.png");
	m_pGameTextures[TextureSheet::BUTTON_OPTIONS] = new Texture("Resources/Images/Menu/ButtonOptions.png");
	m_pGameTextures[TextureSheet::MISSINGNO] = new Texture("Resources/Images/Misc/MissingNo.png");
}

Texture* TextureManager::GetTexture(const TextureSheet textureSheet)
{
	return m_pGameTextures[textureSheet];
}

Texture* TextureManager::GetCharacter(char ascii, const int fontSizeIdx)
{
	return m_pGameFont[fontSizeIdx][ascii];
}