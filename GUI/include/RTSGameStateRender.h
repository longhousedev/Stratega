#pragma once
#include <GameStateRenderer.h>
#include <Stratega/Game/RTSGame.h>
#include <SFML/Window/Window.hpp>
#include <SFML/Graphics.hpp>
#include <CircularBuffer.h>

#include <Widgets/ActionsController.h>

namespace SGA
{
	struct GameConfig;
	struct RenderConfig;
	
	class RTSGameStateRender : public GameStateRenderer<GameState>
	{
	public:
		sf::View view2;

		//Debug mode
		bool drawDebug = false;
		
		RTSGameStateRender(RTSGame& game, const GameConfig& gameConfig, const RenderConfig& renderConfig, int playerID);
		void render() override;

		// GameCommunicator functions
		void onGameStateAdvanced() override;

		bool isSelected(int unitID)
		{
			return actionsSettings.selectedEntities.find(unitID) != actionsSettings.selectedEntities.end();
		}

	private:
		void init() override;
		void init(const GameConfig& gameConfig, const RenderConfig& renderConfig);
		void initializeView(sf::RenderWindow& window) const;
		
		void handleInput(sf::RenderWindow& window);

		//Events Implementation
		void windowClosed(const sf::Event& event, sf::View& view, sf::RenderWindow& window)const;
		void mouseScrolled(const sf::Event& event, sf::View& view, sf::RenderWindow& window);
		void mouseButtonReleased(const sf::Event& event, sf::View& view, sf::RenderWindow& window);
		void mouseButtonPressed(const sf::Event& event, sf::View& view, sf::RenderWindow& window);
		void mouseMoved(const sf::Event& event, sf::View& view, sf::RenderWindow& window);
		void keyPressed(const sf::Event& event, sf::View& view, sf::RenderWindow& window);

		//Render Layers by order
		void drawLayers(sf::RenderWindow& window);

		//HUD
		void createHUD(sf::RenderWindow& window);

		void createTopBar();
		void createBottomBar(sf::RenderWindow& window);
		void createWindowInfo(sf::RenderWindow& window);
		void createWindowUnits();
		void createWindowActionList();
		void createWindowNavMesh();
		void createWindowFogOfWar();
		void createWindowPlayerParameters() const;
		
		void playActions(std::vector<Action> newActionsToPlay)
		{
			for (auto& element : newActionsToPlay)
			{
				//If continous or player action execute directly
				if (element.actionTypeID == -1 || element.isPlayerAction())
				{
					game->executeAction(element);
				}
				else
				{
					//Add actions to list
					actionsToPlay[element.getSourceID()] = element;
				}
			}
		}
	private:
		//Game Data
		RTSGame* game;

		//Current gamestate used to render
		GameState gameStateCopy;
		GameState gameStateCopyFogOfWar;
		//Zoom
		float zoomValue = 5;

		//Mouse Information
		Vector2f currentMousePos;
		sf::Vector2f oldPos;
		bool moving = false;
		bool dragging = false;

		//FPS
		int fpsLimit = 60;
		int m_fps = 0;

		//Drawing gameState Buffer
		bool drawGameStateBuffer = false;
		CircularBuffer<GameState> gameStatesBuffer;
		int gameStatesBufferRCurrentIndex = 0;		

		//Imgui
		sf::Clock deltaClock;

		std::mutex advanceMutex;

		//Profiling
		//Last call OnAdvancedGameGameState
		int indexAdvancedGameState = 0;

		NavigationConfig config;

		sf::RenderTexture renderMinimapTexture;

		//Collect actions played by previous frame
		std::unordered_map<int, Action> actionsToPlay;
	};
}
