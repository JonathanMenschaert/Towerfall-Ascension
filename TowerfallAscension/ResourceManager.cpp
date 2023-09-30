#include "pch.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "GameData.h"

ResourceManager::ResourceManager(const GameData* gameData)
	:m_pTextureManager{new TextureManager()}
	,m_pSoundManager{new SoundManager(gameData->GetSoundSettings())}
{
}

ResourceManager::~ResourceManager()
{
	delete m_pTextureManager;
	m_pTextureManager = nullptr;

	delete m_pSoundManager;
	m_pSoundManager = nullptr;
}

TextureManager* ResourceManager::GetTextureManager() const
{
	return m_pTextureManager;
}

SoundManager* ResourceManager::GetSoundManager() const
{
	return m_pSoundManager;
}
