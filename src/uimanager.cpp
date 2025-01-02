#include "uimanager.hpp"

// CONSTANTS

// COLORS
sf::Color UI_BG = sf::Color(67, 63, 94);

sf::Color DARK_GRAY = sf::Color(100, 100, 100);
sf::Color DARK_GREEN = sf::Color(10, 150, 10);
sf::Color LIGHT_GRAY = sf::Color(180, 180, 180);

// UI AREAS
sf::IntRect endTurnArea(1080, 863, 200, 160);
sf::IntRect mapArea(LEFT_MAP_OFFSET, TOP_MAP_OFFSET,
                    NUM_OF_COLS*(TILE_SIZE + OUTLINE_SIZE),
                    NUM_OF_ROWS*(TILE_SIZE + OUTLINE_SIZE));
sf::IntRect menuArea(LEFT_MAP_OFFSET, 863, WINDOW_WIDTH - LEFT_MAP_OFFSET - 200,
                     160);
// For mainmenu:
sf::IntRect startArea(WINDOW_WIDTH / 2 - 250 / 2,
                      WINDOW_HEIGHT / 2 - 100 / 2 - 50, 250, 100);
sf::IntRect mapeditorArea(WINDOW_WIDTH / 2 - 250 / 2,
                          WINDOW_HEIGHT / 2 - 100 / 2 + 50, 250, 100);

UIManager::UIManager() {
  // Default to main menu
  setState(MAINMENU);
}

void UIManager::setState(UIState state) { state_ = state; }

UIState UIManager::getState() const { return state_; }