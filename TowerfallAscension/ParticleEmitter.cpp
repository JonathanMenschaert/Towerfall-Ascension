#include "pch.h"
#include "ParticleEmitter.h"
#include "Matrix2x3.h"


ParticleEmitter::ParticleEmitter(const Color4f& color, const Rectf& emitter, const Vector2f& directionVector, 
	float jitterAngleMin, float jitterAngleMax, float spawnInterval, float size,
	int amount, bool isActive)
	:m_Color{color}
	,m_Emitter{emitter}
	,m_DirectionVector{directionVector}
	,m_JitterAngleMin{jitterAngleMin}
	,m_JitterAngleMax{jitterAngleMax}
	,m_SpawnInterval{spawnInterval}
	,m_SpawnTimer{}
	,m_IsActive{isActive}
	,m_Amount{amount}
	,m_Size{size}
{
}

ParticleEmitter::~ParticleEmitter()
{
	for (size_t i{}; i < m_pParticles.size(); ++i)
	{
		delete m_pParticles[i];
	}
	m_pParticles.clear();
}

void ParticleEmitter::Update(float elapsedSec)
{
	for (size_t i{}; i < m_pParticles.size(); ++i)
	{
		m_pParticles[i]->Update(elapsedSec);
	}
	if (m_IsActive)
	{
		m_SpawnTimer += elapsedSec;
		if (m_SpawnTimer >= m_SpawnInterval)
		{
			m_SpawnTimer = 0;
			InitParticle();
		}
	}
	RemoveDeadParticle();
}

void ParticleEmitter::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_Emitter.left, m_Emitter.bottom, 0.f);
		for (size_t i{}; i < m_pParticles.size(); ++i)
		{
			m_pParticles[i]->Draw();
		}
	}
	glPopMatrix();
}

void ParticleEmitter::SetEmitter(const Rectf& emitter)
{
	m_Emitter = emitter;
}

void ParticleEmitter::SetActive(bool active)
{
	m_IsActive = active;
}

void ParticleEmitter::InitParticle()
{
	const float lifeTime{ m_SpawnInterval * m_Amount };
	const Point2f position{ float(rand() % int(m_Emitter.width)), float(rand() % int(m_Emitter.height)) };
	int jitterSum{ int(m_JitterAngleMax - m_JitterAngleMin) };
	float angle{};
	if (jitterSum != 0)
	{
		angle = rand() % int(m_JitterAngleMax - m_JitterAngleMin) + m_JitterAngleMin;
	}
	Matrix2x3 matRotate;
	matRotate.SetAsRotate(angle);
	Vector2f speedVector{ matRotate.Transform(m_DirectionVector) };

	m_pParticles.push_back(new Particle(position, m_Size, speedVector, lifeTime, m_Color));
}

void ParticleEmitter::RemoveDeadParticle()
{
	if (m_pParticles.size() > 0)
	{
		if (m_pParticles[0]->ShouldDelete())
		{
			delete m_pParticles[0];
			m_pParticles.pop_front();
		}
	}
}
