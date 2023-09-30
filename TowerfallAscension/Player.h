#pragma once
#include "GameObject.h"
#include "utils.h"
#include "Upgrade.h"

class Sprite;
class AnimatedSprite;
class Keyboard;
class Arrow;
class Level;
class ResourceManager;
class Player final : public GameObject
{
public:

	enum class PlayerState
	{
		IDLE,
		MOVING,
		JUMPING,
		FALLING,
		DASH,
		CROUCH,
		ROLL,
		AIMING,
		HANGING,
		DYING,
		RESPAWN,
		PUSHED,
		GAMEOVER
	};

	Player(const Point2f& position, int lives, int arrows, Keyboard* keyboard, ResourceManager* resources);
	virtual ~Player();
	Player(const Player& c) = delete;
	Player& operator=(const Player& rhs) = delete;
	Player(Player&& c) = delete;
	Player& operator=(Player&& rhs) = delete;

	virtual void Update(float elapsedSec, const Level& level) override;
	virtual void Draw() const override;

	void DrawHud() const;
	Arrow* CheckShotArrow();
	void AddArrow(int amount = 1, bool playSound = true);
	virtual bool AttemptKill() override;

	bool SetUpgrade(Upgrade::UpgradeType upgrade);
	bool IsDead() const;
	bool HasHat() const;
	bool HasUpgrade() const;
	bool HasExploded() const;
	bool IsGameOver() const;
	float GetExplosionRadius() const;

private:

	bool m_IsDead;
	bool m_HasHat;
	bool m_DisableBasicInput;
	bool m_AllowBasicMovement;
	bool m_IsAttachedToWall;
	bool m_PlayRespawnExplosion;
	
	int m_Lives;
	int m_Arrows;
	float m_AccDash;
	float m_DashCooldown;
	float m_AimAngle;
	float m_FlickerTime;
	float m_PushTime;
	float m_Timer;

	float m_LifeGemTimer;

	const int m_MinLives;

	const float m_MaxLifeGemTimer;
	const float m_ExplosionRange;
	const float m_DashTime;
	const float m_MaxDashCooldown;
	const float m_MaxFallSpeed;
	const float m_HorSpeed;
	const float m_JumpSpeed;
	const float m_MaxFlickerTime;
	const float m_MaxPushTime;
	const float m_MaxDeathTime;
	
	const Keyboard* m_pKeyboard;

	Arrow* m_pQuiver;

	Sprite* m_pHeads;
	Sprite* m_pIdle;
	Sprite* m_pDash;
	Sprite* m_pEmptyQuiver;
	Sprite* m_pHanging;
	Sprite* m_pArrow;
	Sprite* m_pAim;
	Sprite* m_pCrouch;
	Sprite* m_pDeath;
	Sprite* m_pRespawn;
	Sprite* m_pAmountArrows;
	

	AnimatedSprite* m_pOutOfArrows;
	AnimatedSprite* m_pBow;
	AnimatedSprite* m_pRunning;
	AnimatedSprite* m_pJumping;	
	AnimatedSprite* m_pRoll;
	AnimatedSprite* m_pShield;
	AnimatedSprite* m_pExplosion;
	AnimatedSprite* m_pLifeGem;

	PlayerState m_PlayerState;
	Upgrade::UpgradeType m_Upgrade;

	void InitPlayerParts(const Point2f& position);
	void SetAimAngle();
	void SetHeadOrientation();

	void UpdateLifeGems(float elapsedSec);
	void UpdateIdle(float elapsedSec, const Level& level);
	void UpdateMoving(float elapsedSec, const Level& level);
	void UpdateJumping(float elapsedSec);
	void UpdateFalling(float elapsedSec);
	void UpdateDash(float elapsedSec);
	void UpdateCrouch(float elapsedSec);
	void UpdateRoll(float elapsedSec, const Level& level);
	void UpdateAiming(float elapsedSec);
	void UpdateHanging(float elapsedSec, const Level& level);
	void UpdateDying(float elapsedSec);
	void UpdateRespawning(float elapsedSec);
	void UpdatePushed(float elapsedSec);
	void UpdateBasicMovement(float elapsedSec, const Level& level);

	void DrawBow() const;
	void DrawHead() const;
	void DrawIdle() const;
	void DrawRunning() const;
	void DrawInAir() const;
	void DrawCrouch() const;
	void DrawDash() const;
	void DrawRoll() const;
	void DrawHanging() const;
	void DrawQuiver() const;
	void DrawDying() const;
	void DrawRespawn() const;
};

