#pragma once
#include "Level.h"
#include "Camera.h"
#include <unordered_map>
#include "Screen.h"

class Player;
class Enemy;
class Keyboard;
class InteractionManager;
class WaveManager;
class UpgradeManager;
class GameData;

class Game final
{
public:
	enum class GameState
	{
		MENU,
		TRANSITION_GAME,
		GAME,		
		PAUSED,
		END
	};
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	int m_CurrentSelectedLevel;
	float m_EndingTimer;
	const float m_MaxEndingTimer;
	GameState m_GameState;
	Rectf m_GameWindow;
	const Window m_Window;
	Player* m_pPlayer;
	Level* m_pLevel;
	Keyboard* m_pKeyboard;
	InteractionManager* m_pInteractionManager;
	WaveManager* m_pWaveManager;
	UpgradeManager* m_pUpgradeManager;
	ResourceManager* m_pResourceManager;

	Screen* m_pMenu;
	Camera m_Camera;
	Sprite* m_pSideTexture;
	Vector2f m_Scale;
	GameData* m_pGameData;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
	void HandleMenu();
	void HandleMapMenu(int buttonValue);
	void SwapMenu(int menuValue);
	void SwapMenu(Screen::ScreenType menuValue);
	void HandlePause(int buttonValue);
	void HandleEnd(int buttonValue);
	void InitializeLevel(int levelIdx);
	void CleanLevel();

	void DrawGame() const;
};