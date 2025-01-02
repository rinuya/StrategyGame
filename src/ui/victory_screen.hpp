#ifndef SRC_VICTORY_SCREEN_HPP_
#define SRC_VICTORY_SCREEN_HPP_

#include <memory>

#include "../gamestate.hpp"
#include "../uimanager.hpp"
#include "../utils.hpp"

class VictoryScreen {
 public:
  /**
   * @brief Construct a new MainMenu object
   *
   * @param uiManager
   * @param font
   * @param window
   */
  VictoryScreen(UIManager& uiManager, sf::Font font,
                std::shared_ptr<sf::RenderWindow> window,
                std::shared_ptr<GameState> gameState);

  /**
   * @brief Displays (draws) the victory screen UI on the render window.
   *
   */
  void displayUI();

  /**
   * @brief Processes the MouseButtonPressed event in the mainmenu UI.
   *
   * @param event
   */
  void processMouseButtonPressed(sf::Event event);

 private:
  UIManager& uiManager_;
  sf::Font font_;
  std::shared_ptr<sf::RenderWindow> window_;
  std::shared_ptr<GameState> gameState_;
};

#endif