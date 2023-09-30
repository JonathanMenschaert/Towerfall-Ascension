#pragma once
#include <vector>
#include <unordered_map>

class Texture;
class TextureManager final
{
public:
	enum class TextureSheet
	{
		//Player Texture Sheets
		PLAYER_BODY,
		PLAYER_HEADS,
		PLAYER_ROLL,
		PLAYER_BOW,
		PLAYER_RESPAWN,
		PLAYER_EXPLOSION,

		//Arrow Textures
		ARROW_AIM,
		ARROW_PLAYER,
		ARROW_QUIVER,

		//Spawn Portal
		SPAWN_PORTAL,
		//Ghost Texture Sheets
		GHOST_WANDERING,
		GHOST_DODGE,
		GHOST_DYING,
		GHOST_CONFUSION,

		//Slime Texture Sheets
		SLIME_MOVEMENT,

		//Particle Texture Sheets

		//Level Texture Sheets
		LEVEL_TORCH,
		SACRED_GROUND_FOREGROUND,
		SACRED_GROUND_BACKGROUND,
		WAVE_NUMBER,

		//Scenery Texture Sheets
		SCENERY_BACKGROUND,
		SCENERY_MOONBACKGROUND,
		SCENERY_MOON,
		SCENERY_SMALLCLOUDS,
		SCENERY_LARGECLOUDS,
		SCENERY_RAIN,

		//HUD
		HUD_SIDE,
		HUD_LIVES,

		//Upgrade Texture Sheets
		UPGRADE_CHEST,
		UPGRADE_SHIELD,
		UPGRADE_SHIELD_AURA,
		UPGRADE_ARROW,
		UPGRADE_HAT,

		//Menu Texture Sheets
		//Start
		START_BACKGROUND,
		START_BANNER,
		START_ARROW,
		START_ORB,
		START_ORBBOTTOM,
		START_TITLE,
		START_TOWER,
		START_LETTERS,

		//Options
		OPTIONS_TOWER,
		

		//Map
		MAP_BACKGROUND,	
		
		//Pause
		PAUSE_BACKGROUND,
		PAUSE_ARROW,

		//EndScreen
		END_BACKGROUND,
		END_PICTURE,

		//Buttons
		MAP_POINTER,
		BUTTON_START,
		BUTTON_SCROLL,
		BUTTON_OPTIONS,

		//No Texture
		MISSINGNO
	};
	TextureManager();	
	~TextureManager();
	TextureManager(const TextureManager& t) = delete;
	TextureManager& operator=(const TextureManager& t) = delete;
	TextureManager(TextureManager&& c) = delete;
	TextureManager& operator=(TextureManager&& rhs) = delete;

	
	Texture* GetTexture(const TextureSheet textureSheet = TextureSheet::MISSINGNO);
	Texture* GetCharacter(char ascii, const int fontSizeIdx);

private:
	std::unordered_map<TextureSheet, Texture*> m_pGameTextures;
	std::vector<std::unordered_map<char, Texture*>> m_pGameFont;
	
	void LoadTextures();
	void LoadGameTextures();
	void LoadGameFont();
};

