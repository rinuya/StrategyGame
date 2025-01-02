#ifndef SRC_UI_HPP_
#define SRC_UI_HPP_

#include <iostream>
#include <sstream>

#include "building.hpp"
#include "gamestate.hpp"
#include "selectable.hpp"
#include "uimanager.hpp"
#include "utils.hpp"

enum Menu { BUILD, ARMY, TOWNHALL };

class UI {
 public:
  // if or once we have networking, this might have to be redone. GameState
  // would reside on a server and send state to the player's client, while the
  // player client would only send user input up to the GameServer

  /**
   * @brief Construct a new UI object
   *
   * @param player
   * @param gameState
   * @param window
   */
  UI(std::shared_ptr<Player> player, std::shared_ptr<GameState> gameState,
     sf::Font font, std::shared_ptr<sf::RenderWindow> window);

  /**
   * @brief Displays (draws) a player specific UI on the render window.
   *
   */
  void displayUI();

  /**
   * @brief Processes the MouseButtonPressed event in the players UI.
   *
   * @param event
   */
  void processMouseButtonPressed(sf::Event event);

 private:
  // UI ELEMENTS

  void displayResources();

  void displaySelected();

  void displayEndRound();

  void displayInfoBox();

  void displayMap();

  void displayHighlightedtiles();

  void displayMenu();

  void displayWinner();

  // GAME PIECES

  void drawTile(const Tile& tile, int xPos, int yPos,
                bool isHighlighted = false);

  void drawBuilding(const Building& building, int xPos, int yPos);

  void drawTownhall(const TownHall& townhall, int xPos, int yPos);

  void drawArmy(const Army& army, int xPos, int yPos);

  void displayArmyMovementRange();

  void drawArmyMovementSuggestionMarker(const Army& army, int xPos, int yPos);

  // HELPERS

  void addHighlightedTile(int xPos, int yPos);

  void removeHighlightedTile(int xPos, int yPos);

  void processMenuSelected(sf::Event event);

  void processTileSelected(int xPos, int yPos);

  Selectable* getSelected() const;

  void setSelected(Selectable* selectable);

  void resetSelected();

  // PRIVATE VARS

  std::shared_ptr<sf::RenderWindow> window_;
  std::shared_ptr<Player> player_;
  std::shared_ptr<GameState> gameState_;

  Menu activeMenu_;
  // selected_;
  std::vector<Tile*> highlightedTiles_;

  Selectable* selected_ = nullptr;

  // UI constants
  sf::Font font;
};

#endif  // SRC_UI_HPP_
