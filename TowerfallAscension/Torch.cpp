#include "pch.h"
#include "Torch.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "ParticleEmitter.h"
Torch::Torch(const Point2f& position, const ResourceManager* resources)
	:m_pTorchTexture{new Sprite(resources->GetTextureManager()->GetTexture(TextureManager::TextureSheet::LEVEL_TORCH)) }
{
	Vector2f scale{ 3.5f, 3.5f };
	m_pTorchTexture->SetScale(scale);
	m_Shape = Rectf{ position.x - m_pTorchTexture->GetScaledWidth() / 2.f, position.y - m_pTorchTexture->GetScaledHeight() / 2.f,
		m_pTorchTexture->GetScaledWidth(), m_pTorchTexture->GetScaledHeight() };
	Rectf emitter{(3 * m_Shape.width / 8.f), m_Shape.height, m_Shape.width / 4.f, m_Shape.height / 4.f };
	const float jitterMin{ -10.f }, jitterMax{ 10.f };
	m_pFireEmitter.push_back(new ParticleEmitter(Color4f{ 203.f / 255.f, 23.f / 255.f, 8.f / 255.f, 1.f }, emitter, Vector2f{ 0.f, 15.f }, jitterMin, jitterMax, 0.05f, 5.f, 20));
	m_pFireEmitter.push_back(new ParticleEmitter(Color4f{ 238.f / 255.f, 242.f / 255.f, 76.f / 255.f, 1.f }, emitter, Vector2f{ 0.f, 3.f }, jitterMin, jitterMax, 0.05f, 5.f, 20));
}

Torch::~Torch()
{
	for (size_t i{}; i < m_pFireEmitter.size(); ++i)
	{
		delete m_pFireEmitter[i];
	}
	m_pFireEmitter.clear();

	delete m_pTorchTexture;
	m_pTorchTexture = nullptr;
}

void Torch::Update(float elapsedSec)
{

	for (size_t i{}; i < m_pFireEmitter.size(); ++i)
	{
		m_pFireEmitter[i]->Update(elapsedSec);
	}
}

void Torch::Draw() const
{
	Rectf emitter{ m_Shape.left + m_Shape.width / 4.f, m_Shape.bottom + m_Shape.height / 2.f, m_Shape.width / 2.f, m_Shape.height / 2.f };
	
	glPushMatrix();
	{
		glTranslatef(m_Shape.left, m_Shape.bottom, 0.f);
		m_pTorchTexture->Draw();
		for (size_t i{}; i < m_pFireEmitter.size(); ++i)
		{
			m_pFireEmitter[i]->Draw();
		}
	}
	glPopMatrix();
	//utils::DrawRect(emitter);
}
