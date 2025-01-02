#ifndef SRC_MAINMENU_HPP_
#define SRC_MAINMENU_HPP_

#include <memory>

#include "../uimanager.hpp"
#include "../utils.hpp"

class MainMenu {
 public:
  /**
   * @brief Construct a new MainMenu object
   *
   * @param uiManager
   * @param font
   * @param window
   */
  MainMenu(UIManager& uiManager, sf::Font font,
           std::shared_ptr<sf::RenderWindow> window);

  /**
   * @brief Displays (draws) the mainmenu UI on the render window.
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
};

#endif