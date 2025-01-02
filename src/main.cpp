#include <filesystem>
#include <fstream>
#include <vector>

#include "gamestate.hpp"
#include "player.hpp"
#include "playerAI.hpp"
#include "tile.hpp"
#include "ui.hpp"
#include "ui/mainmenu.hpp"
#include "ui/mapeditor.hpp"
#include "ui/victory_screen.hpp"
#include "uimanager.hpp"

// constants for map moved to uimanager.hpp

// TODO should later be moved to a utils.cpp folder or sth
std::string readFileToString(const std::string& filename);

int main() {
  auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1280, 1024),
                                                   "Strategy-8");
  window->setFramerateLimit(144);

  std::shared_ptr<GameState> gs = std::make_shared<GameState>(window);
  UIManager uiManager = UIManager();
  //!!!TEMP  modified player Colors: (just so you can see that the tile is
  //! highlighted)
  std::shared_ptr<Player> player1 =
      std::make_shared<Player>(sf::Color(0, 128, 128), 1);
  // std::shared_ptr<PlayerAI> player1 =
  // std::make_shared<PlayerAI>(sf::Color(0,128,128), 1, gs);

  std::shared_ptr<PlayerAI> player2 =
      std::make_shared<PlayerAI>(sf::Color(0, 0, 128), 2, gs);

  // player1->addSharedPtr(player1);
  player2->addSharedPtr(player2);

  gs->addPlayer(player1);
  gs->addPlayer(player2);

  // create the map
  std::string filename = "maps/example1.txt";
  std::string fileContent = readFileToString(filename);

  // for debugging purposes, map file has to be in /build/maps/map_file_name.txt
  // you can also edit the map file directly in the build directory and won't
  // need to rebuild the game
  std::filesystem::path fullPath = std::filesystem::absolute(filename);
  std::cout << "Accessing file: " << fullPath << std::endl;

  gs->loadMapFromString(fileContent, NUM_OF_ROWS);

  // claim some test tiles
  gs->claimTile(4, 4, player1);
  gs->claimTile(3, 4, player1);
  gs->claimTile(5, 4, player1);
  gs->claimTile(4, 3, player1);
  gs->claimTile(4, 5, player1);

  gs->claimTile(18, 15, player2);
  gs->claimTile(17, 15, player2);
  gs->claimTile(19, 15, player2);
  gs->claimTile(18, 14, player2);
  gs->claimTile(18, 16, player2);

  // gs->claimTile(5, 5, player2);
  // gs->claimTile(6, 7, player1);
  // gs->claimTile(8, 8, player2);

  // place some test Townhalls
  TownHall townhall1(1, 1, player1, 4, 4);
  TownHall townhall2(2, 1, player2, 18, 15);
  gs->addTownhall(townhall1);
  gs->addTownhall(townhall2);

  // place some test armies
  gs->addArmy(ARTILLERY, 1, 1, player1, 4);
  gs->addArmy(CAVALRY, 1, 2, player2, 100);
  // gs->addArmy(CAVALRY, 1, 1, player1, 300);
  gs->addArmy(CAVALRY, 17, 14, player1, 300);
  // gs->addArmy(6, 6, player1, 3);
  // gs->addArmy(7, 6, player1, 2);
  // gs->addArmy(5, 7, player1, 12);
  // gs->addArmy(8, 7, player1, 9);
  // gs->addArmy(11, 6, player1, 14);
  // gs->addArmy(13, 6, player1, 25);
  // gs->addArmy(6, 11, player1, 1);
  // gs->addArmy(6, 12, player1, 1);
  // gs->addArmy(0, 0, player1, 4);
  // gs->addArmy(7, 11, player2, 1);
  // gs->addArmy(7, 12, player2, 1);
  // gs->addArmy(6, 7, player2, 4);
  // gs->addArmy(8, 8, player2, 7);
  // gs->addArmy(12, 6, player2, 14);
  // gs->addArmy(14, 6, player2, 128);
  // gs->addArmy(20, 19, player2, 4);

  // move test armies
  // Army& army1_1 = gs->getArmyByLocation(6, 6);
  // gs->moveArmy(army1_1, 7, 6);
  // Army& army1_3 = gs->getArmyByLocation(5, 7);
  // gs->moveArmy(army1_3, 6, 7);
  // Army& army2_2 = gs->getArmyByLocation(8, 8);
  // gs->moveArmy(army2_2, 8, 7);
  // Army& army1_5 = gs->getArmyByLocation(11, 6);
  // gs->moveArmy(army1_5, 12, 6);
  // Army& army2_4 = gs->getArmyByLocation(14, 6);
  // gs->moveArmy(army2_4, 13, 6);

  sf::Font f;
  if (!f.loadFromFile("HackNerdFont-Regular.ttf")) {
    std::cerr << "Error loading font!" << std::endl;
  }

  MainMenu mainmenu = MainMenu(uiManager, f, window);

  MapEditor mapeditor = MapEditor(uiManager, f, window);

  UI player1ui = UI(player1, gs, f, window);
  UI player2ui = UI(player2, gs, f, window);

  VictoryScreen victoryScreen(uiManager, f, window, gs);

  gs->setActivePlayerID(player2->getID());

  while (window->isOpen()) {
    int currentPlayerID = gs->getActivePlayerID();
    UIState uiState = uiManager.getState();

    for (auto event = sf::Event(); window->pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window->close();
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        if (uiState == MAINMENU) {
          mainmenu.processMouseButtonPressed(event);
        } else if (uiState == GAME) {
          if (player1->getID() == currentPlayerID)
            player1ui.processMouseButtonPressed(event);

          else if (player2->getID() == currentPlayerID)
            player2ui.processMouseButtonPressed(event);

          // if we have more than 2 players, we can expand here ..
        } else if (uiState == VICTORY) {
          victoryScreen.processMouseButtonPressed(event);
        } else if (uiState == MAPEDITOR) {
          mapeditor.processMouseButtonPressed(event);
        }
      }
    }

    if (uiState == GAME && gs->getWinner()) uiManager.setState(VICTORY);

    window->clear();
    // All drawing has to happen here between clear and display.
    // Preferably calling a function elsewhere.

    if (uiState == MAINMENU) {
      mainmenu.displayUI();
    } else if (uiState == GAME) {
      if (player1->getID() == currentPlayerID)
        player1ui.displayUI();

      else if (player2->getID() == currentPlayerID)
        player2ui.displayUI();
    } else if (uiState == VICTORY) {
      victoryScreen.displayUI();
    } else if (uiState == MAPEDITOR) {
      mapeditor.displayUI();
    }

    window->display();
  }
}

std::string readFileToString(const std::string& filename) {
  std::ifstream file(filename);
  std::string line;
  std::string content;

  if (!file.is_open()) {
    std::cerr << "Could not open the file: " << filename << std::endl;
    return "";
  }
  // Read each line from the file and append it to the content string
  while (std::getline(file, line)) {
    content += line;
  }

  file.close();
  return content;
}