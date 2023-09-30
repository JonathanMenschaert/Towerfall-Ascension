#pragma once

class TextureManager;
class SoundManager;
class GameData;
class ResourceManager final
{
public:
	ResourceManager(const GameData* gameData);
	~ResourceManager();
	ResourceManager(const ResourceManager& c) = delete;
	ResourceManager& operator=(const ResourceManager& rhs) = delete;
	ResourceManager(ResourceManager&& c) = delete;
	ResourceManager& operator=(ResourceManager&& rhs) = delete;
	TextureManager* GetTextureManager() const;
	SoundManager* GetSoundManager() const;

private:
	TextureManager* m_pTextureManager;
	SoundManager* m_pSoundManager;
};

