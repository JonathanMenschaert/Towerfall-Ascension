#pragma once
#include <vector>
#include "utils.h"
class ResourceManager;
class ParticleEmitter;
class Sprite;

class Torch final
{
public:
	Torch(const Point2f& position, const ResourceManager* resources);
	~Torch();
	Torch(const Torch& c) = delete;
	Torch& operator=(const Torch& rhs) = delete;
	Torch(Torch&& c) = delete;
	Torch& operator=(Torch&& rhs) = delete;

	void Update(float elapsedSec);
	void Draw() const;

private:

	std::vector<ParticleEmitter*> m_pFireEmitter;
	Sprite* m_pTorchTexture;
	Rectf m_Shape;
};

