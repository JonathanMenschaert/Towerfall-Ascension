#include "pch.h"
#include "Player.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Texture.h"
#include <iostream>
#include "Keyboard.h"
#include "Arrow.h"
#include "Level.h"
#include "SoundManager.h"

Player::Player(const Point2f& position, int lives, int arrows, Keyboard* keyboard, ResourceManager* resources)
	:GameObject(resources)
	,m_pKeyboard{keyboard}
	,m_Lives{lives}
	,m_Arrows{arrows}
	,m_PlayerState{PlayerState::IDLE}
	,m_MaxFallSpeed{-500.f}
	,m_HorSpeed{250.f}
	,m_JumpSpeed{600.f}
	,m_DashTime{0.1f}
	,m_AccDash{0.f}
	,m_MaxDashCooldown{1.f}
	,m_DashCooldown{1.f}
	,m_HasHat{true}
	,m_AimAngle{0.f}
	,m_pQuiver{nullptr}
	,m_Upgrade{Upgrade::UpgradeType::NONE}
	,m_MaxFlickerTime{0.5f}
	,m_FlickerTime{0.f}
	,m_AllowBasicMovement{true}
	,m_IsAttachedToWall{false}
	,m_DisableBasicInput{false}
	,m_MaxDeathTime{2.0f}
	,m_MaxPushTime{0.15f}
	,m_Timer{}
	,m_IsDead{false}
	,m_PlayRespawnExplosion{true}
	,m_ExplosionRange{100.f}
	,m_MaxLifeGemTimer{2.5f}
	,m_LifeGemTimer{0.f}
	,m_MinLives{0}
{
	m_Velocity = Vector2f{ m_HorSpeed, 0.f };
	InitPlayerParts(position);
}

void Player::InitPlayerParts(const Point2f& position)
{
	Vector2f scale{ m_Scale, m_Scale };

	TextureManager* textureManager{ m_pResources->GetTextureManager() };

	Texture* pHeads{ textureManager->GetTexture(TextureManager::TextureSheet::PLAYER_HEADS) };
	m_pHeads = new Sprite{ pHeads, Rectf{0.f, 5 * pHeads->GetHeight() / 8, pHeads->GetWidth() / 4, pHeads->GetHeight() / 8} };
	m_pHeads->SetScale(scale);

	Texture* pBow{ textureManager->GetTexture(TextureManager::TextureSheet::PLAYER_BOW) };
	m_pBow = new AnimatedSprite{ pBow, Rectf{0.f, pBow->GetHeight() / 2, pBow->GetWidth() / 3, pBow->GetHeight() / 2}, 
		1, 3, 3, 3, AnimatedSprite::AnimationState::FREEZE};
	m_pBow->SetScale(scale);

	m_pEmptyQuiver = new AnimatedSprite{ pBow, Rectf{0.f, pBow->GetHeight(), pBow->GetWidth() / 3, pBow->GetHeight() / 2},
		1, 2, 3, 2, AnimatedSprite::AnimationState::FREEZE };
	m_pEmptyQuiver->SetScale(scale);

	Texture* pBody{ textureManager->GetTexture(TextureManager::TextureSheet::PLAYER_BODY) };
	Texture* pRoll{ textureManager->GetTexture(TextureManager::TextureSheet::PLAYER_ROLL) };

	m_pIdle = new Sprite{ pBody, Rectf{0.f, pBody->GetHeight(), pBody->GetWidth() / 12, pBody->GetHeight()} };
	m_pIdle->SetScale(scale);

	m_pRunning = new AnimatedSprite(pBody, Rectf{ pBody->GetWidth() / 12, pBody->GetHeight(), 
		pBody->GetWidth() / 12, pBody->GetHeight() },
		1, 2, 6, 2, AnimatedSprite::AnimationState::LOOP);
	m_pRunning->SetScale(scale);

	m_pJumping = new AnimatedSprite(pBody, Rectf{ pBody->GetWidth() / 3, pBody->GetHeight(), 
		pBody->GetWidth() / 12, pBody->GetHeight() },
		1, 4, 1, 4, AnimatedSprite::AnimationState::FREEZE);
	m_pJumping->SetScale(scale);

	m_pDash = new Sprite(pBody, Rectf{ 0.75f * pBody->GetWidth(), pBody->GetHeight(), 
		pBody->GetWidth() / 12, pBody->GetHeight() });
	m_pDash->SetScale(scale);

	m_pHanging = new Sprite(pBody, Rectf{ 0.25f * pBody->GetWidth(), pBody->GetHeight(), 
		pBody->GetWidth() / 12, pBody->GetHeight() });
	m_pHanging->SetScale(scale);

	m_pCrouch = new Sprite(pBody, Rectf{ 5.f * pBody->GetWidth() / 6.f, pBody->GetHeight(), 
		pBody->GetWidth() / 12, pBody->GetHeight() });
	m_pCrouch->SetScale(scale);

	m_pRoll = new AnimatedSprite(pRoll, Rectf{ 0.f, pRoll->GetHeight(), pRoll->GetWidth() / 8, pRoll->GetHeight() / 2 },
		1, 8, 24, 8, AnimatedSprite::AnimationState::FREEZE);
	m_pRoll->SetScale(scale);

	m_pAim = new Sprite(textureManager->GetTexture(TextureManager::TextureSheet::ARROW_AIM));
	m_pAim->SetScale(scale);
	
	Texture* pAura{ textureManager->GetTexture(TextureManager::TextureSheet::UPGRADE_SHIELD_AURA) };
	m_pShield = new AnimatedSprite(pAura, Rectf{ 0.f, pAura->GetHeight(), pAura->GetWidth() / 7.f, pAura->GetHeight() },
		1, 7, 8, 7, AnimatedSprite::AnimationState::LOOP);
	m_pShield->SetScale(scale);

	Texture* pQuiver{ textureManager->GetTexture(TextureManager::TextureSheet::ARROW_QUIVER) };
	m_pAmountArrows = new Sprite(pQuiver, Rectf{ 0.f, pQuiver->GetHeight() / 2.f, 
		pQuiver->GetWidth() / 4.f, pQuiver->GetHeight() / 2.f });
	m_pAmountArrows->SetScale(scale * 0.8f);

	m_pOutOfArrows = new AnimatedSprite(pQuiver, Rectf{ 0.f, pQuiver->GetHeight(), 
		pQuiver->GetWidth() / 2.f, pQuiver->GetHeight() / 2.f },
		1, 2, 20, 2, AnimatedSprite::AnimationState::LOOP);
	m_pOutOfArrows->SetScale(scale);

	Texture* pRespawn{ textureManager->GetTexture(TextureManager::TextureSheet::PLAYER_RESPAWN) };
	m_pDeath = new Sprite(pRespawn, Rectf{ 0.f, pRespawn->GetHeight(), pRespawn->GetWidth() / 6.f, pRespawn->GetHeight() });
	m_pDeath->SetScale(scale);

	m_pRespawn = new Sprite(pRespawn, Rectf{ pRespawn->GetWidth() / 3.f, pRespawn->GetHeight(), pRespawn->GetWidth() / 6.f, pRespawn->GetHeight() });
	m_pRespawn->SetScale(scale);

	Texture* pExplosion{ textureManager->GetTexture(TextureManager::TextureSheet::PLAYER_EXPLOSION) };
	m_pExplosion = new AnimatedSprite(pExplosion, Rectf{ 0.f, pExplosion->GetHeight() / 2.f, pExplosion->GetWidth() / 4.f, pExplosion->GetHeight() / 2.f },
		2, 4, 8, 8, AnimatedSprite::AnimationState::LOOP_END);
	m_pExplosion->SetScale(scale);

	Texture* pGem{ textureManager->GetTexture(TextureManager::TextureSheet::HUD_LIVES) };
	m_pLifeGem = new AnimatedSprite(pGem, Rectf{ 0.f, pGem->GetHeight() / 2.f, pGem->GetWidth() / 4.f, pGem->GetHeight() / 2.f },
		2, 4, 10, 7, AnimatedSprite::AnimationState::LOOP_ONCE);
	m_pLifeGem->SetScale(scale);

	m_Shape = Rectf{ position.x, position.y, pBody->GetWidth() / 12 * scale.x , pBody->GetHeight() * scale.y };
}

Player::~Player()
{
	delete m_pHeads;
	m_pHeads = nullptr;

	delete m_pBow;
	m_pBow = nullptr;

	delete m_pIdle;
	m_pIdle = nullptr;

	delete m_pRunning;
	m_pRunning = nullptr;

	delete m_pEmptyQuiver;
	m_pEmptyQuiver = nullptr;

	delete m_pJumping;
	m_pJumping = nullptr;

	delete m_pRoll;
	m_pRoll = nullptr;

	delete m_pDash;
	m_pDash = nullptr;

	delete m_pHanging;
	m_pHanging = nullptr;

	delete m_pCrouch;
	m_pCrouch = nullptr;

	delete m_pAim;
	m_pAim = nullptr;

	delete m_pQuiver;
	m_pQuiver = nullptr;

	delete m_pShield;
	m_pShield = nullptr;

	delete m_pAmountArrows;
	m_pAmountArrows = nullptr;

	delete m_pOutOfArrows;
	m_pOutOfArrows = nullptr;

	delete m_pDeath;
	m_pDeath = nullptr;

	delete m_pRespawn;
	m_pRespawn = nullptr;

	delete m_pExplosion;
	m_pExplosion = nullptr;

	delete m_pLifeGem;
	m_pLifeGem = nullptr;
}

Arrow* Player::CheckShotArrow()
{	
	if (m_pQuiver)
	{
		Arrow* pArrow = m_pQuiver;
		m_pQuiver = nullptr;
		return pArrow;
	}
	return nullptr;
}

void Player::Update(float elapsedSec, const Level& level)
{
	if (m_DashCooldown < m_MaxDashCooldown)
	{
		m_DashCooldown += elapsedSec;
	}
	switch (m_Upgrade)
	{
	case Upgrade::UpgradeType::SHIELD:
		m_pShield->Update(elapsedSec);
		break;
	default:
		break;
	}
	
	m_AllowBasicMovement = true;
	m_IsAttachedToWall = false;
	switch (m_PlayerState)
	{
	case PlayerState::IDLE:
		UpdateIdle(elapsedSec, level);
		break;
	case PlayerState::MOVING:
		UpdateMoving(elapsedSec, level);
		break;
	case PlayerState::JUMPING:
		UpdateJumping(elapsedSec);
		break;
	case PlayerState::FALLING:
		UpdateFalling(elapsedSec);
		break;
	case PlayerState::CROUCH:
		UpdateCrouch(elapsedSec);
		break;
	case PlayerState::DASH:
		UpdateDash(elapsedSec);
		break;
	case PlayerState::ROLL:
		UpdateRoll(elapsedSec, level);
		break;
	case PlayerState::HANGING:
		UpdateHanging(elapsedSec, level);
		break;
	case PlayerState::AIMING:
		UpdateAiming(elapsedSec);
		break;
	case PlayerState::PUSHED:
		UpdatePushed(elapsedSec);
		break;
	case PlayerState::DYING:
		UpdateDying(elapsedSec);
		break;
	case PlayerState::RESPAWN:
		UpdateRespawning(elapsedSec);
		break;
	case PlayerState::GAMEOVER:
		break;
	}

	UpdateBasicMovement(elapsedSec, level);

	if (m_FlickerTime > 0.f)
	{
		m_pOutOfArrows->Update(elapsedSec);
		m_FlickerTime -= elapsedSec;
	}

	level.HandleCollision(m_Shape, m_Velocity, m_IsAttachedToWall);
	SetAimAngle();
	SetHeadOrientation();
	HandleLevelBoundaries();
	UpdateLifeGems(elapsedSec);
}

void Player::AddArrow(int amount, bool playSound)
{
	m_Arrows += amount;
	if (playSound)
	{
		m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_ARROW_RECOVER);
	}
}

void Player::SetAimAngle()
{
	bool isLeftPressed{ m_pKeyboard->IsMoveLeftPressed()  };
	bool isRightPressed{ m_pKeyboard->IsMoveRightPressed() };
	bool isUpPressed{ m_pKeyboard->IsUpPressed() };
	bool isDownPressed{ m_pKeyboard->IsCrouchPressed() };

	float prevAim{ m_AimAngle };

	if (isDownPressed && (isLeftPressed || isRightPressed))
	{
		m_AimAngle = -45.f;
	}
	else if (isUpPressed && (isLeftPressed || isRightPressed))
	{
		m_AimAngle = 45.f;
	}
	else if (isUpPressed)
	{
		m_AimAngle = 90.f;
	}
	else if (isDownPressed)
	{
		m_AimAngle = -90.f;
	}
	else
	{
		m_AimAngle = 0.f;
	}	
	if (m_PlayerState == PlayerState::AIMING && m_AimAngle != prevAim)
	{
		m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_AIM_CHANGE);
	}
}

void Player::SetHeadOrientation()
{
	int widthMod{};
	switch (int(m_AimAngle))
	{
	case 90:
	case 45:
		widthMod = 1;
		break;
	case -45:
	case -90:
		widthMod = 2;
		break;
	default:
		widthMod = 0;
		break;
	}

	int heightMod{(m_HasHat ? 5 : 1)};
	if (m_Velocity.y > 0.f)
	{
		heightMod += 3;
	}
	else if(m_Velocity.y < m_MaxFallSpeed * 0.1f)
	{
		heightMod += 2;
	}
	else if (m_Velocity.y < m_MaxFallSpeed * 0.5f)
	{
		heightMod += 1;
	}

	Point2f pos{ widthMod * m_pHeads->GetWidth(), heightMod * m_pHeads->GetHeight() };
	m_pHeads->SetClipPosition(pos);
}

void Player::UpdateLifeGems(float elapsedSec)
{
	m_pLifeGem->Update(elapsedSec);
	m_LifeGemTimer += elapsedSec;
	if (m_LifeGemTimer >= m_MaxLifeGemTimer)
	{
		m_pLifeGem->StartAnimation();
		m_LifeGemTimer = 0.f;
	}
}

void Player::UpdateIdle(float elapsedSec, const Level& level)
{
	if (level.IsOnGround(m_Shape, m_Velocity))
	{
		if (m_pKeyboard->IsMoveLeftPressed() || m_pKeyboard->IsMoveRightPressed())
		{
			m_PlayerState = PlayerState::MOVING;
		}
		else if (m_pKeyboard->IsJumpPressed())
		{
			m_PlayerState = PlayerState::JUMPING;
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_JUMP);
		}
		else if (m_pKeyboard->IsCrouchPressed())
		{
			m_PlayerState = PlayerState::CROUCH;
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_CROUCH);
		}
		else if (m_pKeyboard->IsDashPressed() && m_DashCooldown >= m_MaxDashCooldown)
		{
			m_PlayerState = PlayerState::DASH;
		}
		else if (m_pKeyboard->IsShootPressed())
		{
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_AIM);
			m_PlayerState = PlayerState::AIMING;
		}
	}
}

void Player::UpdateMoving(float elapsedSec, const Level& level)
{
	m_pRunning->Update(elapsedSec);
	if (level.IsOnGround(m_Shape, m_Velocity))
	{
		if (!(m_pKeyboard->IsMoveLeftPressed() || m_pKeyboard->IsMoveRightPressed()))
		{
			m_PlayerState = PlayerState::IDLE;
		}
		else if (m_pKeyboard->IsJumpPressed())
		{
			m_PlayerState = PlayerState::JUMPING;
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_JUMP);
		}
		else if (m_pKeyboard->IsDashPressed() && m_DashCooldown >= m_MaxDashCooldown)
		{
			m_PlayerState = PlayerState::DASH;
		}
		else if (m_pKeyboard->IsShootPressed())
		{
			m_PlayerState = PlayerState::AIMING;
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_AIM);
		}
	}
	else
	{
		if (m_Velocity.y <= -20.f)
		{
			m_PlayerState = PlayerState::FALLING;
			m_pJumping->NextFrame(2);
		}
	}
}

void Player::UpdateJumping(float elapsedSec)
{
	if (m_Velocity.y < m_MaxFallSpeed * 0.1f)
	{
		m_PlayerState = PlayerState::FALLING;
		m_pJumping->NextFrame();
	}
	else if (m_Velocity.y == 0.f)
	{
		m_pJumping->NextFrame(0);
		m_Velocity.y = m_JumpSpeed;
	}

	if (m_pKeyboard->IsDashPressed() && m_DashCooldown >= m_MaxDashCooldown)
	{
		m_PlayerState = PlayerState::DASH;
		m_Velocity.y = 0.f;
	}

	if (m_Velocity.y < m_JumpSpeed * 0.2f && m_pJumping->GetCurrentFrame() == 0)
	{
		m_pJumping->NextFrame();
	}
}

void Player::UpdateFalling(float elapsedSec)
{
	if (m_Velocity.y == 0)
	{
		m_PlayerState = PlayerState::MOVING;
		m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_LAND);
	}
	if (m_pKeyboard->IsDashPressed() && m_DashCooldown >= m_MaxDashCooldown)
	{
		m_Velocity.y = 0.f;
		m_PlayerState = PlayerState::DASH;
	}
	if (m_Velocity.y < m_MaxFallSpeed * 0.5f && m_pJumping->GetCurrentFrame() == 2)
	{
		m_pJumping->NextFrame();
	}
}

void Player::UpdateDash(float elapsedSec)
{
	m_AccDash += elapsedSec;
	if (m_AccDash >= m_DashTime)
	{
		m_DashCooldown = 0.f;
		m_AccDash = 0.f;
		m_PlayerState = PlayerState::MOVING;
	}
	else
	{
		m_Shape.left += 3.f * (m_Velocity.x > 0 ? m_HorSpeed : -m_HorSpeed) * elapsedSec;
	}
	m_AllowBasicMovement = false;
}

void Player::UpdateCrouch(float elapsedSec)
{
	if (m_pKeyboard->IsCrouchPressed())
	{
		m_AllowBasicMovement = false;
		if (m_pKeyboard->IsDashPressed() && m_DashCooldown >= m_MaxDashCooldown)
		{
			m_PlayerState = PlayerState::ROLL;
			m_pRoll->SetClipPosition(Point2f{ 0.f, (m_HasHat ? 2.f : 1.f) * m_pRoll->GetHeight() });
			m_pRoll->StartAnimation();
		}
	}
	else
	{
		m_PlayerState = PlayerState::MOVING;
	}
}

void Player::UpdateRoll(float elapsedSec, const Level& level)
{
	m_pRoll->Update(elapsedSec);
	if (m_pRoll->GetCurrentFrame() != m_pRoll->GetTotalFrames() - 1)
	{
		m_Shape.left += 2.5f * (m_Velocity.x > 0 ? m_HorSpeed : -m_HorSpeed) * elapsedSec;
	}
	else
	{
		m_DashCooldown = 0.f;
		if (level.IsOnGround(m_Shape, m_Velocity))
		{
			m_PlayerState = PlayerState::CROUCH;
		}
		else
		{
			m_PlayerState = PlayerState::MOVING;
		}
		m_pRoll->NextFrame();
	}
	m_AllowBasicMovement = false;
}

void Player::UpdateAiming(float elapsedSec)
{
	if (!m_pKeyboard->IsShootPressed())
	{
		if (m_AimAngle != -90.f)
		{
			if (m_Arrows > 0)
			{
				m_pQuiver = new Arrow(Point2f{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 },
					(m_Velocity.x > 0 ? m_AimAngle : 180.f - m_AimAngle), m_pResources);
				--m_Arrows;
			}
			else
			{
				m_FlickerTime = m_MaxFlickerTime;
				m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_NOFIRE);
			}
		}
		
		m_PlayerState = PlayerState::IDLE;
	}
	if (m_pKeyboard->IsMoveRightPressed())
	{
		if (m_Velocity.x < 0.f)
		{
			m_Velocity.x = m_HorSpeed * elapsedSec;
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_AIM_CHANGE);
		}
	}
	else if (m_pKeyboard->IsMoveLeftPressed())
	{
		if (m_Velocity.x > 0.f)
		{
			m_Velocity.x = -m_HorSpeed * elapsedSec;
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_AIM_CHANGE);
		}
	}
	else if (!m_pKeyboard->IsUpPressed() && m_pKeyboard->IsCrouchPressed())
	{
		m_PlayerState = PlayerState::IDLE;
		m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_AIM_CANCEL);
	}
	m_AllowBasicMovement = false;
}

void Player::UpdateHanging(float elapsedSec, const Level& level)
{
	if ((m_pKeyboard->IsMoveLeftPressed() || m_pKeyboard->IsMoveRightPressed())
		&& level.IsTouchingWall(m_Shape, m_Velocity) && !level.IsOnGround(m_Shape, m_Velocity))
	{
		m_IsAttachedToWall = true;
		if (m_pKeyboard->IsJumpPressed() && m_Velocity.y > -200.f)
		{
			m_PlayerState = PlayerState::JUMPING;
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_JUMP);
		}
	}
	else
	{
		m_PlayerState = PlayerState::MOVING;
	}
}

void Player::UpdateDying(float elapsedSec)
{
	if (m_Velocity.y == 0.f)
	{		
		if (m_Timer >= m_MaxDeathTime)
		{			
			--m_Lives;
			m_Timer = 0.f;
			if (m_Lives > m_MinLives)		
			{				
				m_HasHat = false;				
				m_Velocity.y = 200.f;
				m_pExplosion->NextFrame(0);
				m_PlayerState = PlayerState::RESPAWN;
				m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_RESPAWN);
			}
			else
			{
				m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_GAMEOVER);
				m_PlayerState = PlayerState::GAMEOVER;
			}
		}
		else
		{
			m_Timer += elapsedSec;
		}
	}
}

void Player::UpdateRespawning(float elapsedSec)
{
	if (m_Velocity.y <= 0.f)
	{
		m_AllowBasicMovement = false;
		m_pExplosion->Update(elapsedSec);
		if (m_PlayRespawnExplosion && m_pExplosion->GetCurrentFrame() == m_pExplosion->GetTotalFrames() / 2)
		{
			m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_RESPAWN_EXPLOSION);
			m_PlayRespawnExplosion = false;
		}
		else if (m_pExplosion->GetCurrentFrame() == m_pExplosion->GetTotalFrames() - 1)
		{			
			m_PlayerState = PlayerState::IDLE;
			m_PlayRespawnExplosion = true;
			m_DisableBasicInput = false;
			m_IsDead = false;
		}
	}
}

void Player::UpdatePushed(float elapsedSec)
{
	m_Timer += elapsedSec;
	if (m_Timer >= m_MaxPushTime)
	{
		m_DisableBasicInput = false;
		m_Timer = 0.f;
		m_PlayerState = PlayerState::IDLE;
	}
	else
	{
		m_Shape.left += m_Velocity.x * elapsedSec;
	}
}

void Player::UpdateBasicMovement(float elapsedSec, const Level& level)
{
	if (m_AllowBasicMovement)
	{
		if (m_pKeyboard->IsMoveLeftPressed() && !m_DisableBasicInput)
		{
			m_Velocity.x = -m_HorSpeed * elapsedSec;
			m_Shape.left += m_Velocity.x;
			if (m_PlayerState != PlayerState::JUMPING && m_PlayerState != PlayerState::HANGING && level.IsTouchingWall(m_Shape, m_Velocity)
				&& !level.IsOnGround(m_Shape, m_Velocity))
			{
				m_PlayerState = PlayerState::HANGING;
				m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_GRAB);
			}
		}
		else if (m_pKeyboard->IsMoveRightPressed() && !m_DisableBasicInput)
		{
			m_Velocity.x = m_HorSpeed * elapsedSec;
			m_Shape.left += m_Velocity.x;
			if (m_PlayerState != PlayerState::JUMPING && m_PlayerState != PlayerState::HANGING && m_Velocity.y != 0.f
				&& level.IsTouchingWall(m_Shape, m_Velocity) && !level.IsOnGround(m_Shape, m_Velocity))
			{
				m_PlayerState = PlayerState::HANGING;
				m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_GRAB);
			}
		}

		if (!(m_PlayerState == PlayerState::HANGING && m_Velocity.y == 0.f))
		{
			m_Velocity += m_Acceleration * elapsedSec;
		}

		if (m_Velocity.y <= m_MaxFallSpeed)
		{
			m_Velocity.y = m_MaxFallSpeed;
		}
		m_Shape.bottom += m_Velocity.y * elapsedSec;
	}
}

void Player::Draw() const
{
	CopyDirection copyState{ m_CopySprite };

	int amount{};
	if (copyState == CopyDirection::NONE)
	{
		amount = 1;
	}
	else
	{
		amount = 2;
	}
	for (int i{}; i < amount; ++i)
	{
		glPushMatrix();

		switch (copyState)
		{
		case CopyDirection::DOWN:
			glTranslatef(0.f, -m_LevelBoundaries.height, 0.f);
			break;
		case CopyDirection::UP:
			glTranslatef(0.f, m_LevelBoundaries.height, 0.f);
			break;
		case CopyDirection::LEFT:
			glTranslatef(-m_LevelBoundaries.width, 0.f, 0.f);
			break;
		case CopyDirection::RIGHT:
			glTranslatef(m_LevelBoundaries.width, 0.f, 0.f);
			break;
		default:
			break;
		}

		if (m_Velocity.x < 0)
		{
			glTranslatef(m_Shape.left + m_Shape.width, m_Shape.bottom, 0.f);
			glScalef(-1.f, 1.f, 1.f);
		}
		else
		{
			glTranslatef(m_Shape.left, m_Shape.bottom, 0.f);
		}

		switch (m_Upgrade)
		{
		case Upgrade::UpgradeType::SHIELD:
			glPushMatrix();
			{
				glScalef(0.75f, 0.75f, 1.f);
				glTranslatef(-m_pShield->GetWidth() / 2.f * m_Scale + m_Shape.width * 0.7f,
					-m_pShield->GetHeight() / 2.f * m_Scale + m_Shape.height * 0.7f, 0.f);
				m_pShield->Draw();
			}
			glPopMatrix();
			break;
		default:
			break;
		}
		
		switch (m_PlayerState)
		{
		case PlayerState::PUSHED:
		case PlayerState::AIMING:
		case PlayerState::IDLE:
			DrawIdle();
			DrawQuiver();
			break;
		case PlayerState::MOVING:
			DrawRunning();
			DrawQuiver();
			break;
		case PlayerState::JUMPING:
			DrawInAir();
			DrawQuiver();
			break;
		case PlayerState::FALLING:
			DrawInAir();
			DrawQuiver();
			break;
		case PlayerState::CROUCH:
			DrawCrouch();
			DrawQuiver();
			break;
		case PlayerState::DASH:			
			DrawDash();
			DrawQuiver();
			break;
		case PlayerState::ROLL:
			DrawRoll();
			DrawQuiver();
			break;
		case PlayerState::HANGING:
			DrawHanging();
			DrawQuiver();
			break;
		case PlayerState::GAMEOVER:
		case PlayerState::DYING:
			DrawDying();
			break;
		case PlayerState::RESPAWN:
			DrawRespawn();
			break;
		}
		
		glPopMatrix();
		copyState = CopyDirection::NONE;
	}
}

void Player::DrawHud() const
{
	glPushMatrix();
	{
		glTranslatef(m_LevelBoundaries.left, m_LevelBoundaries.height - m_pLifeGem->GetScaledHeight() * 1.5f, 0.f);
		for (int i{}; i < m_Lives; ++i)
		{			
			m_pLifeGem->Draw();
			glTranslatef(m_pLifeGem->GetScaledWidth() * 0.75f, 0.f, 0.f);
		}
		
	}
	glPopMatrix();
}

void Player::DrawBow() const
{
	glPushMatrix();
	{		
		glTranslatef(m_Shape.width * 0.5f, m_Shape.height * 0.6f, 0.f);
		glRotatef(m_AimAngle, 0.f, 0.f, 1.f);
		
		if (m_PlayerState == PlayerState::AIMING)
		{
			m_pAim->Draw(Vector2f{ m_pAim->GetWidth() * 0.1f, -m_pAim->GetHeight() * 0.5f });
		}

		if (m_Arrows > 0)
		{
			m_pBow->Draw(Vector2f{ -m_pBow->GetWidth() * 0.15f, -m_pBow->GetHeight() * 0.5f });
		}
		else
		{
			m_pEmptyQuiver->Draw(Vector2f{ -m_pEmptyQuiver->GetWidth() * 0.15f, -m_pEmptyQuiver->GetHeight() * 0.5f });
		}
		
	}
	glPopMatrix();
}

void Player::DrawHead() const
{

	glPushMatrix();
	{
		glTranslatef(m_Shape.width / 12, 0.6f * m_Shape.height, 0.f);

		m_pHeads->Draw();
	}
	glPopMatrix();
}

void Player::DrawQuiver() const
{
	glPushMatrix();
	{
		if (m_Arrows > 0)
		{
			glTranslatef(m_Shape.width / 2.f - m_Scale * 0.8f * m_pAmountArrows->GetWidth() * m_Arrows / 2.f,
				m_Shape.height * 1.2f, 0.f);
			for (int i{}; i < m_Arrows; ++i)
			{
				m_pAmountArrows->Draw();
				glTranslatef(m_Scale * m_pAmountArrows->GetWidth() * 0.8f, 0.f, 0.f);
			}
		}
		else if (m_FlickerTime > 0.f)
		{
			glTranslatef(m_Shape.width / 2.f - m_Scale * m_pOutOfArrows->GetWidth() / 2.f,
				m_Shape.height * 1.2f, 0.f);
			m_pOutOfArrows->Draw();
		}
		
	}
	glPopMatrix();
}

void Player::DrawIdle() const
{
	m_pIdle->Draw();
	DrawHead();
	DrawBow();
}

void Player::DrawRunning() const
{
	m_pRunning->Draw();
	glPushMatrix();
	{
		glTranslatef(0.f, m_Shape.height * -0.05f * m_pRunning->GetCurrentFrame(), 0.f);
		DrawHead();
		DrawBow();
	}
	glPopMatrix();
}

void Player::DrawInAir() const
{
	m_pJumping->Draw();
	DrawHead();
	DrawBow();
}

void Player::DrawCrouch() const
{
	m_pCrouch->Draw();
	glPushMatrix();
	{
		glTranslatef(0.f, -m_Shape.height * 0.2f, 0.f);
		DrawHead();
	}
	glPopMatrix();
}

void Player::DrawRoll() const
{
	m_pRoll->Draw();
}

void Player::DrawDash() const
{
	m_pDash->Draw();
	DrawHead();
}

void Player::DrawHanging() const
{
	m_pHanging->Draw();
	DrawHead();
}

void Player::DrawDying() const
{
	m_pDeath->Draw();
}

void Player::DrawRespawn() const
{	
	glPushMatrix();
	{
		glScalef(0.65f, 0.65f, 1.f);
		glTranslatef(m_Shape.width / 2.f - m_pExplosion->GetWidth() / 2.f * m_Scale,
			m_Shape.height / 2.f - m_pExplosion->GetHeight() / 2.f * m_Scale, 0.f);
		m_pExplosion->Draw();

	}
	glPopMatrix();
	m_pRespawn->Draw();
}

bool Player::AttemptKill()
{
	if (m_Upgrade == Upgrade::UpgradeType::SHIELD)
	{
		m_Timer = 0.f;
		m_PlayerState = PlayerState::PUSHED;
		m_Upgrade = Upgrade::UpgradeType::NONE;
		m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::POWERUP_SHIELD_DISABLED);
		m_DisableBasicInput = true;
		return true;
	}
	else if (m_PlayerState != PlayerState::DYING && m_PlayerState != PlayerState::PUSHED && m_PlayerState != PlayerState::RESPAWN
		&& m_PlayerState != PlayerState::DASH && m_PlayerState != PlayerState::ROLL)
	{
		m_IsDead = true;
		m_DisableBasicInput = true;
		m_PlayerState = PlayerState::DYING;
		m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::PLAYER_DEATH);
		return true;
	}
	return false;
}

bool Player::SetUpgrade(Upgrade::UpgradeType upgrade)
{	
	switch (upgrade)
	{
	case Upgrade::UpgradeType::ARROWS:
		AddArrow(2, false);
		m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::POWERUP_ARROW);
		break;
	case Upgrade::UpgradeType::HAT:
		m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::POWERUP_HAT);
		m_HasHat = true;
		break;
	case Upgrade::UpgradeType::SHIELD:
		if (m_Upgrade != Upgrade::UpgradeType::NONE)
		{			
			return false;
		}
		m_pResources->GetSoundManager()->PlaySoundEffect(SoundManager::SoundEffectType::POWERUP_SHIELD);
		m_Upgrade = upgrade;
		break;
	}
	return true;
}

bool Player::IsDead() const
{
	return m_IsDead;
}

bool Player::HasHat() const
{
	return m_HasHat;
}

bool Player::HasUpgrade() const
{
	return m_Upgrade != Upgrade::UpgradeType::NONE;
}

bool Player::HasExploded() const
{
	return !m_PlayRespawnExplosion;
}

bool Player::IsGameOver() const
{
	return m_Lives == m_MinLives;
}

float Player::GetExplosionRadius() const
{
	return m_ExplosionRange;
}
