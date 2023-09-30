#include "pch.h"
#include "SoundManager.h"
#include "SoundStream.h"
#include "SoundEffect.h"
#include <string>
#include "GameData.h"

SoundManager::SoundManager(const SoundSettings& options)
	:m_SfxVolume{options.sfxVolume}
	,m_MusicVolume{options.musicVolume}
{
	LoadSounds();
}

SoundManager::~SoundManager()
{
	for (size_t i{}; i < m_pMusicTracks.size(); ++i)
	{
		delete m_pMusicTracks[Music(i)];
	}

	for (size_t i{}; i < m_pSoundEffects.size(); ++i)
	{
		delete m_pSoundEffects[SoundEffectType(i)];
	}
}

void SoundManager::LoadSounds()
{
	LoadMusicTracks();
	LoadSoundEffects();
}

void SoundManager::LoadMusicTracks()
{
	m_pMusicTracks[Music::MAINMENU] = new SoundStream("Resources/Sounds/Music/MainMenu.mp3");
	m_pMusicTracks[Music::ARCHIVES] = new SoundStream("Resources/Sounds/Music/Archives.mp3");
	m_pMusicTracks[Music::SACRED_GROUND] = new SoundStream("Resources/Sounds/Music/SacredGround.mp3");
	m_pMusicTracks[Music::FALLEN] = new SoundStream("Resources/Sounds/Music/Fallen.mp3");
	m_pMusicTracks[Music::VICTORY] = new SoundStream("Resources/Sounds/Music/Victory.mp3");
}

void SoundManager::LoadSoundEffects()
{
	
	m_pSoundEffects[SoundEffectType::GHOST_DODGE] = new SoundEffect("Resources/Sounds/SFX/GhostDodge.wav");
	m_pSoundEffects[SoundEffectType::GHOST_DEATH] = new SoundEffect("Resources/Sounds/SFX/GhostDeath.wav");
	m_pSoundEffects[SoundEffectType::SLIME_DEATH] = new SoundEffect("Resources/Sounds/SFX/SlimeDeath.wav");
	m_pSoundEffects[SoundEffectType::SLIME_LAND] = new SoundEffect("Resources/Sounds/SFX/SlimeLand.wav");
	m_pSoundEffects[SoundEffectType::LIGHTNING_1] = new SoundEffect("Resources/Sounds/SFX/Lightning1.wav");
	m_pSoundEffects[SoundEffectType::LIGHTNING_2] = new SoundEffect("Resources/Sounds/SFX/Lightning2.wav");
	m_pSoundEffects[SoundEffectType::LIGHTNING_3] = new SoundEffect("Resources/Sounds/SFX/Lightning3.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_ARROW_RECOVER] = new SoundEffect("Resources/Sounds/SFX/PlayerArrowRecover.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_AIM] = new SoundEffect("Resources/Sounds/SFX/PlayerAim.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_AIM_CANCEL] = new SoundEffect("Resources/Sounds/SFX/PlayerAimCancel.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_AIM_CHANGE] = new SoundEffect("Resources/Sounds/SFX/PlayerAimChange.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_DEATH] = new SoundEffect("Resources/Sounds/SFX/PlayerDeath.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_GRAB] = new SoundEffect("Resources/Sounds/SFX/PlayerGrab.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_JUMP] = new SoundEffect("Resources/Sounds/SFX/PlayerJump.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_LAND] = new SoundEffect("Resources/Sounds/SFX/PlayerLand.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_NOFIRE] = new SoundEffect("Resources/Sounds/SFX/PlayerNoFire.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_RESPAWN] = new SoundEffect("Resources/Sounds/SFX/PlayerRespawn.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_RESPAWN_EXPLOSION] = new SoundEffect("Resources/Sounds/SFX/PlayerRespawnExplosion.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_CROUCH] = new SoundEffect("Resources/Sounds/SFX/PlayerCrouch.wav");
	m_pSoundEffects[SoundEffectType::PLAYER_GAMEOVER] = new SoundEffect("Resources/Sounds/SFX/PlayerGameOver.wav");
	m_pSoundEffects[SoundEffectType::CHEST_APPEAR] = new SoundEffect("Resources/Sounds/SFX/ChestAppear.wav");
	m_pSoundEffects[SoundEffectType::CHEST_OPEN] = new SoundEffect("Resources/Sounds/SFX/ChestOpen.wav");
	m_pSoundEffects[SoundEffectType::POWERUP_ARROW] = new SoundEffect("Resources/Sounds/SFX/PowerupArrow.wav");
	m_pSoundEffects[SoundEffectType::POWERUP_SHIELD] = new SoundEffect("Resources/Sounds/SFX/PowerupShield.wav");
	m_pSoundEffects[SoundEffectType::POWERUP_SHIELD_DISABLED] = new SoundEffect("Resources/Sounds/SFX/PowerupShieldDisabled.wav");
	m_pSoundEffects[SoundEffectType::POWERUP_HAT] = new SoundEffect("Resources/Sounds/SFX/PowerupHat.wav");
	m_pSoundEffects[SoundEffectType::PORTAL_SPAWN] = new SoundEffect("Resources/Sounds/SFX/PortalSpawn.wav");
	m_pSoundEffects[SoundEffectType::PORTAL_DISAPPEAR] = new SoundEffect("Resources/Sounds/SFX/PortalDisappear.wav");
	m_pSoundEffects[SoundEffectType::PORTAL_APPEAR] = new SoundEffect("Resources/Sounds/SFX/PortalAppear.wav");
	m_pSoundEffects[SoundEffectType::UI_PAUSE] = new SoundEffect("Resources/Sounds/SFX/UIPause.wav");
	m_pSoundEffects[SoundEffectType::UI_UNPAUSE] = new SoundEffect("Resources/Sounds/SFX/UIUnPause.wav");
}

void SoundManager::PlayTrack(SoundManager::Music track, bool repeat)
{
	const SoundStream* musicTrack = m_pMusicTracks[track];
	musicTrack->Play(repeat);
	musicTrack->SetVolume(m_MusicVolume);
}

void SoundManager::ResumeTrack()
{
	SoundStream::Resume();
}

void SoundManager::PauseTrack()
{
	SoundStream::Pause();
}

void SoundManager::StopTrack()
{
	SoundStream::Stop();
}

void SoundManager::SetTrackVolume(int volume)
{
	m_MusicVolume = volume;
	SoundStream::SetVolume(volume);
}

int SoundManager::GetTrackVolume()
{
	return m_MusicVolume;
}

void SoundManager::PlaySoundEffect(SoundManager::SoundEffectType soundEffect, int amountRepeat)
{
	SoundEffect* effect = m_pSoundEffects[soundEffect];
	effect->Play(amountRepeat);
	effect->SetVolume(m_SfxVolume);
}

void SoundManager::PauseAllSoundEffects()
{
	SoundEffect::PauseAll();
}

void SoundManager::ResumeAllSoundEffects()
{
	SoundEffect::ResumeAll();
}

void SoundManager::StopAllSoundEffects()
{
	SoundEffect::StopAll();
}

void SoundManager::SetSoundEffectVolume(int volume)
{
	m_SfxVolume = volume;
	for (size_t i{}; i < m_pSoundEffects.size(); ++i)
	{
		SoundEffect* sound = m_pSoundEffects[SoundEffectType(i)];
		if (sound)
		{
			sound->SetVolume(volume);
		}
	}
}

int SoundManager::GetSoundEffectVolume()
{
	return m_SfxVolume;
}


std::string SoundManager::ToString()
{
	std::string properties{};
	properties += "SfxVolume=\"" + std::to_string(GetSoundEffectVolume()) + "\"\n";
	properties += "MusicVolume=\"" + std::to_string(GetTrackVolume()) + "\"\n";
	return properties;
}
