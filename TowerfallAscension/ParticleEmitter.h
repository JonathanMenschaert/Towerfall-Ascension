#pragma once
#include <deque>
#include "Particle.h"

class Texture;
class ParticleEmitter final
{
public:

	ParticleEmitter(const Color4f& color, const Rectf& emitter, const Vector2f& directionVector, 
		float jitterAngleMin, float jitterAngleMax, float spawnInterval, float size, 
		int amount = 1, bool isActive = true);
	~ParticleEmitter();
	ParticleEmitter(const ParticleEmitter& c) = delete;
	ParticleEmitter& operator=(const ParticleEmitter& rhs) = delete;
	ParticleEmitter(ParticleEmitter&& c) = delete;
	ParticleEmitter& operator=(ParticleEmitter&& rhs) = delete;

	void Update(float elapsedSec);
	void Draw() const;

	void SetEmitter(const Rectf& emitter);
	void SetActive(bool active);

private:

	int m_Amount;

	bool m_IsActive;
	float m_SpawnTimer;
	float m_Size;
	const float m_SpawnInterval;
	std::deque<Particle*> m_pParticles;

	Rectf m_Emitter;

	const Vector2f m_DirectionVector;
	const float m_JitterAngleMin, m_JitterAngleMax;
	const Color4f m_Color;

	void InitParticle();
	void RemoveDeadParticle();
};

