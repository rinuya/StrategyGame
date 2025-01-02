#ifndef SRC_UIMANAGER_HPP_
#define SRC_UIMANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <iostream>

// CONSTANTS

enum UIState { MAINMENU, GAME, MAPEDITOR, VICTORY };

// WINDOW SIZE
constexpr const int WINDOW_WIDTH = 1280;
constexpr const int WINDOW_HEIGHT = 1024;

// MAP SPECIFIC VALUES
constexpr const int NUM_OF_COLS = 25;
constexpr const int NUM_OF_ROWS = 20;

// UI OFFSETS
constexpr const int TOP_MAP_OFFSET = 64;
constexpr const int LEFT_MAP_OFFSET = 280;

constexpr const float TILE_SIZE = 38;
constexpr const float OUTLINE_SIZE = 2;

// COLORS
extern sf::Color UI_BG;

extern sf::Color DARK_GRAY;
extern sf::Color DARK_GREEN;
extern sf::Color LIGHT_GRAY;

// UI AREAS
extern sf::IntRect endTurnArea;
extern sf::IntRect mapArea;
extern sf::IntRect menuArea;
// For mainmenu:
extern sf::IntRect startArea;
extern sf::IntRect mapeditorArea;

class UIManager {
 public:
  /**
   * @brief Construct a new UIManager object
   */
  UIManager();

  /**
   * set the UIState of this UIManager.
   * @param state the new UIState enum value for this UIManager.
   **/
  void setState(UIState state);

  /**
   * get this UIManager's current UIState.
   * @return This UIManager's UIState.
   **/
  UIState getState() const;

 private:
  UIState state_;
};

#endif