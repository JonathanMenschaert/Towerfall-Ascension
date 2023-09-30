#pragma once

#include <unordered_map>

class SoundStream;
class SoundEffect;
struct SoundSettings;
class SoundManager final
{
public:
	enum class SoundEffectType
	{
		LIGHTNING_1,
		LIGHTNING_2,
		LIGHTNING_3,

		PLAYER_ARROW_RECOVER,
		PLAYER_AIM,
		PLAYER_AIM_CANCEL,
		PLAYER_AIM_CHANGE,
		PLAYER_DEATH,
		PLAYER_GRAB,
		PLAYER_JUMP,
		PLAYER_LAND,
		PLAYER_NOFIRE,
		PLAYER_RESPAWN,
		PLAYER_RESPAWN_EXPLOSION,
		PLAYER_CROUCH,
		PLAYER_GAMEOVER,

		GHOST_DODGE,
		GHOST_DEATH,

		SLIME_DEATH,
		SLIME_LAND,

		CHEST_OPEN,
		CHEST_APPEAR,

		POWERUP_ARROW,
		POWERUP_SHIELD,
		POWERUP_SHIELD_DISABLED,
		POWERUP_HAT,

		PORTAL_APPEAR,
		PORTAL_DISAPPEAR,
		PORTAL_SPAWN,

		UI_PAUSE,
		UI_UNPAUSE
	};
	enum class Music
	{
		MAINMENU,
		ARCHIVES,
		SACRED_GROUND,
		FALLEN,
		VICTORY
	};
	SoundManager(const SoundSettings& options);
	~SoundManager();
	SoundManager(const SoundManager& c) = delete;
	SoundManager& operator=(const SoundManager& rhs) = delete;
	SoundManager(SoundManager&& c) = delete;
	SoundManager& operator=(SoundManager&& rhs) = delete;

	void PlayTrack(SoundManager::Music track, bool repeat = true);
	void ResumeTrack();
	void PauseTrack();
	void StopTrack();

	void SetTrackVolume(int volume);
	int GetTrackVolume();

	void PlaySoundEffect(SoundManager::SoundEffectType soundEffect, int amountRepeat = 0);
	void PauseAllSoundEffects();
	void ResumeAllSoundEffects();
	void StopAllSoundEffects();

	void SetSoundEffectVolume(int volume);
	int GetSoundEffectVolume();

	std::string ToString();

private:
	
	int m_SfxVolume;
	int m_MusicVolume;

	std::unordered_map<SoundManager::Music, const SoundStream*> m_pMusicTracks;
	std::unordered_map<SoundManager::SoundEffectType, SoundEffect*> m_pSoundEffects;

	void LoadSounds();
	void LoadMusicTracks();
	void LoadSoundEffects();
};

