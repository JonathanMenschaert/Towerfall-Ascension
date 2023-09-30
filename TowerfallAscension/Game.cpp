#include "pch.h"
#include "Game.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "AnimatedSprite.h"
#include "Player.h"
#include "Ghost.h"
#include "Slime.h"
#include "Keyboard.h"
#include <iostream>
#include "Keyboard.h"
#include "InteractionManager.h"
#include "WaveManager.h"
#include "SpawnTimer.h"
#include "UpgradeManager.h"
#include <iostream>
#include "StartScreen.h"
#include "MapScreen.h"
#include "Screen.h"
#include "PauseScreen.h"
#include "GameData.h"
#include "EndScreen.h"
#include "OptionsScreen.h"

Game::Game( const Window& window ) 
	:m_Window{ window }
	,m_Camera{window.width, window.height}
	,m_pKeyboard{new Keyboard()}
	,m_GameState{GameState::MENU}
	,m_pMenu{}
	,m_Scale{ m_Window.width / 1920.f, m_Window.height / 1080.f }
	,m_pInteractionManager{nullptr}
	,m_pUpgradeManager{nullptr}
	,m_pLevel{nullptr}
	,m_pWaveManager{nullptr}
	,m_pPlayer{nullptr}
	, m_CurrentSelectedLevel{}
	, m_EndingTimer{}
	, m_MaxEndingTimer{3.f}
{
	Initialize( );
	
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{	
	m_pGameData = new GameData("Resources/Config/Root.txt" );
	m_pResourceManager = new ResourceManager(m_pGameData);
	Vector2f translation{ Vector2f{ (1920.f - 1440.f) / 2.f * m_Scale.x, 0.f } };
	m_GameWindow = Rectf{ translation.x, translation.y, m_Scale.x * 1440, m_Scale.y * 1080 };

	m_pMenu = new StartScreen(m_Window.width, m_Window.height, m_pResourceManager);
	//m_pMenu = new OptionsScreen(m_Window.width, m_Window.height, m_pResourceManager);

	m_Camera.SetLevelBoundaries(m_pMenu->GetMenuBoundaries());

	m_pSideTexture = new Sprite(m_pResourceManager->GetTextureManager()->GetTexture(TextureManager::TextureSheet::HUD_SIDE));
	m_pSideTexture->SetScale(m_Scale);
}

void Game::Cleanup( )
{
	CleanLevel();

	delete m_pKeyboard;
	m_pKeyboard = nullptr;

	delete m_pSideTexture;
	m_pSideTexture = nullptr;

	delete m_pMenu;
	m_pMenu = nullptr;

	m_pGameData->SaveOptions(m_pResourceManager);

	delete m_pGameData;
	m_pGameData = nullptr;

	delete m_pResourceManager;
	m_pResourceManager = nullptr;
}


void Game::CleanLevel()
{
	delete m_pPlayer;
	m_pPlayer = nullptr;

	delete m_pLevel;
	m_pLevel = nullptr;

	delete m_pInteractionManager;
	m_pInteractionManager = nullptr;

	delete m_pWaveManager;
	m_pWaveManager = nullptr;

	delete m_pUpgradeManager;
	m_pUpgradeManager = nullptr;
}

void Game::Update( float elapsedSec )
{
	switch (m_GameState)
	{
	case GameState::END:
	case GameState::MENU:
	case GameState::PAUSED:
 		m_pMenu->Update(elapsedSec);
		break;
	case GameState::TRANSITION_GAME:
		m_pMenu->Update(elapsedSec);
		if (m_Camera.CanZoomIn())
		{
			m_Camera.ZoomIn(0.2f);
		}
		else
		{
			InitializeLevel(m_CurrentSelectedLevel);
			m_GameState = GameState::GAME;
			m_Camera.SetLevelBoundaries(m_pLevel->GetBoundaries());
			m_Camera.ZoomReset();			
			SwapMenu(Screen::ScreenType::PAUSE);
		}
		break;
	case GameState::GAME:
	{
		m_pPlayer->Update(elapsedSec, *m_pLevel);
		Arrow* pArrow{ m_pPlayer->CheckShotArrow() };
		if (pArrow)
		{
			m_pInteractionManager->ShootArrow(pArrow);
		}
		m_pWaveManager->Update(elapsedSec, *m_pInteractionManager, *m_pUpgradeManager, *m_pLevel);
		m_pUpgradeManager->Update(elapsedSec, *m_pLevel);
		m_pUpgradeManager->AttemptOpenChest(m_pPlayer->GetActorShape());
		m_pUpgradeManager->AttemptPickupUpgrade(*m_pPlayer, *m_pKeyboard);
		m_pInteractionManager->Update(elapsedSec, *m_pLevel, *m_pPlayer, *m_pUpgradeManager);
		m_pLevel->Update(elapsedSec);
		
		if (m_pWaveManager->HasReachedEnd() || m_pPlayer->IsGameOver())
		{			
			if (m_EndingTimer >= m_MaxEndingTimer)
			{
				m_GameState = GameState::END;				
			}
			else if (m_EndingTimer <= 0.f)
			{
				delete m_pMenu;
				m_pMenu = new EndScreen(m_Window.width, m_Window.height, m_pResourceManager, !m_pPlayer->IsGameOver());
			}			
			m_EndingTimer += elapsedSec;	
		}
	}
		break;
	}
}

void Game::Draw( ) const
{
	ClearBackground( );
	switch (m_GameState)
	{
	case GameState::TRANSITION_GAME:
	case GameState::MENU:
		glPushMatrix();
		{
			m_Camera.Transform(m_pMenu->GetWidgetPosition());
			m_pMenu->Draw();
		}
		glPopMatrix();
		m_pMenu->DrawHud();
		break;
	case GameState::GAME:		
		DrawGame();
		m_pPlayer->DrawHud();
		break;
	
	case GameState::END:
	case GameState::PAUSED:
		DrawGame();
		m_pMenu->Draw();
		break;
	}	
	glPushMatrix();
	{
		m_pSideTexture->Draw();
		glTranslatef(m_GameWindow.left + m_GameWindow.width, 0.f, 0.f);
		m_pSideTexture->Draw();
	}
	glPopMatrix();
}

void Game::DrawGame() const
{
	m_pLevel->DrawScenery();

	glPushMatrix();
	{
		m_Camera.Transform(m_pPlayer->GetActorShape());
		m_pLevel->DrawBackground();
		m_pUpgradeManager->Draw();
		m_pInteractionManager->Draw();
		m_pPlayer->Draw();
		m_pLevel->DrawForeground();
	}
	glPopMatrix();

	m_pWaveManager->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	m_pMenu->ProcessKeyPress(m_pKeyboard);
	if (m_pMenu->IsKeyPressed())
	{
		HandleMenu();
	}

	
	switch (e.keysym.sym)
	{
	case SDLK_KP_PLUS:
		if (m_GameState == GameState::GAME)
		{
			m_Camera.ZoomIn();
		}
		break;
	case SDLK_KP_MINUS:
		if (m_GameState == GameState::GAME)
		{
			m_Camera.ZoomOut();
		}
		break;
	case SDLK_i:
		std::cout << *m_pKeyboard << std::endl;
	}
	
}

void Game::HandleMenu()
{
	int buttonValue{ m_pMenu->GetButtonValue() };
	switch (m_pMenu->GetMenuType())
	{
	case Screen::ScreenType::MAP:
		HandleMapMenu(buttonValue);
		break;
	case Screen::ScreenType::PAUSE:
		HandlePause(buttonValue);
		break;
	case Screen::ScreenType::ENDING:
		HandleEnd(buttonValue);
		break;
	case Screen::ScreenType::OPTIONS:
	default:
		SwapMenu(buttonValue);
		break;
	}
}
void Game::HandleMapMenu(int buttonValue)
{
	if (buttonValue >= 0)
	{
		m_CurrentSelectedLevel = buttonValue;
		m_GameState = GameState::TRANSITION_GAME;
	}
	else
	{
		SwapMenu(Screen::ScreenType::START);
	}
}

void Game::SwapMenu(int menuValue)
{
	SwapMenu(Screen::ScreenType(menuValue));
}

void Game::SwapMenu(Screen::ScreenType menuValue)
{
	
	switch (menuValue)
	{
	case Screen::ScreenType::START:
	{
		m_Camera.ZoomReset();
		m_GameState = GameState::MENU;
		delete m_pMenu;
		m_pMenu = new StartScreen(m_Window.width, m_Window.height, m_pResourceManager);
		m_Camera.SetLevelBoundaries(m_pMenu->GetMenuBoundaries());
		break;
	}
	case Screen::ScreenType::MAP:
	{
		m_Camera.ZoomReset();
		m_GameState = GameState::MENU;
		delete m_pMenu;
		m_pMenu = new MapScreen(m_Window.width, m_Window.height, m_pGameData->GetMapPointers(), 
			m_pGameData->GetLevelNames(), m_pResourceManager);
		m_Camera.SetLevelBoundaries(m_pMenu->GetMenuBoundaries());
		break;
	}
	case Screen::ScreenType::PAUSE:	
	{
		m_Camera.ZoomReset();
		delete m_pMenu;
		m_pMenu = new PauseScreen(m_Window.width, m_Window.height, m_pResourceManager);
		break;
	}
	case Screen::ScreenType::OPTIONS:
		m_Camera.ZoomReset();
		m_GameState = GameState::MENU;
		delete m_pMenu;
		m_pMenu = new OptionsScreen(m_Window.width, m_Window.height, m_pResourceManager);
	default:
		break;
	}
}

void Game::HandlePause(int buttonValue)
{
	switch (PauseScreen::ListValue(buttonValue))
	{
	case PauseScreen::ListValue::ESCAPE:

		if (m_GameState == GameState::GAME)
		{
			m_GameState = GameState::PAUSED;
			return;
		}
	case PauseScreen::ListValue::RESUME:
		m_GameState = GameState::GAME;
		break;
	case PauseScreen::ListValue::RESTART:

		m_GameState = GameState::GAME;
		InitializeLevel(m_CurrentSelectedLevel);
		break;
	case PauseScreen::ListValue::MAP: 

		SwapMenu(Screen::ScreenType::MAP);
		CleanLevel();
		break;
	case PauseScreen::ListValue::QUIT:

		delete m_pMenu;
		m_pMenu = new StartScreen(m_Window.width, m_Window.height, m_pResourceManager);
		SwapMenu(Screen::ScreenType::START);
		CleanLevel();
		break;
	}
}

void Game::HandleEnd(int buttonValue)
{
	switch (EndScreen::ListValue(buttonValue))
	{
	case EndScreen::ListValue::RESTART:

		m_GameState = GameState::GAME;
		InitializeLevel(m_CurrentSelectedLevel);
		SwapMenu(Screen::ScreenType::PAUSE);
		break;
	case EndScreen::ListValue::MAP:
		SwapMenu(Screen::ScreenType::MAP);
		CleanLevel();
		break;
	case EndScreen::ListValue::QUIT:

		delete m_pMenu;
		m_pMenu = new StartScreen(m_Window.width, m_Window.height, m_pResourceManager);
		SwapMenu(Screen::ScreenType::START);
		CleanLevel();
		break;
	}
}

void Game::InitializeLevel(int levelIdx)
{
	CleanLevel();

	m_EndingTimer = 0.f;
	const LevelTemplate& levelTemplate{ m_pGameData->GetLevelTemplateById(levelIdx) };
	m_pLevel = new Level(m_Window, levelTemplate.m_Level, m_pResourceManager);

	m_pPlayer = new Player(levelTemplate.m_Level.playerSpawn, levelTemplate.m_Player.lives, levelTemplate.m_Player.arrows, m_pKeyboard, m_pResourceManager);

	m_pInteractionManager = new InteractionManager(m_pLevel->GetBoundaries(), m_pResourceManager);

	m_pWaveManager = new WaveManager(m_Window, levelTemplate.m_Enemies, m_pResourceManager);

	m_pUpgradeManager = new UpgradeManager(levelTemplate.m_Chests, m_pResourceManager);
	m_pPlayer->SetLevelBoundaries(m_pLevel->GetBoundaries());
}


void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
	{
		std::cout << "[DEBUG] Mouse Clicked at: " << float(e.x) << ", " << float(e.y) << std::endl;
		break;
	}
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
