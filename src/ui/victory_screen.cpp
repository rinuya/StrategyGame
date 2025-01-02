#include "victory_screen.hpp"

sf::IntRect quitArea(WINDOW_WIDTH / 2 - 250 / 2,
                     WINDOW_HEIGHT / 2 - 100 / 2 + 50, 250, 100);

VictoryScreen::VictoryScreen(UIManager& uiManager, sf::Font font,
                             std::shared_ptr<sf::RenderWindow> window,
                             std::shared_ptr<GameState> gameState)
    : uiManager_(uiManager),
      font_(font),
      window_(window),
      gameState_(gameState) {}

void VictoryScreen::displayUI() {
  std::string winnerName = "";
  std::shared_ptr<Player> winner = gameState_->getWinner();
  if (winner) winnerName = "Player " + std::to_string(winner->getID());
  sf::RectangleShape messageRect =
      createRect(250, 100, WINDOW_WIDTH / 2 - 250 / 2,
                 WINDOW_HEIGHT / 2 - 100 / 2 - 50, sf::Color::Transparent);
  sf::Text messageText =
      createTextForRect(messageRect, font_, winnerName + " won!");

  window_->draw(messageText);

  sf::RectangleShape quitButton =
      createRect(250, 100, quitArea.left, quitArea.top, sf::Color::Green);
  sf::Text quitText = createTextForRect(quitButton, font_, "Quit");

  window_->draw(quitButton);
  window_->draw(quitText);
}

void VictoryScreen::processMouseButtonPressed(sf::Event event) {
  if (quitArea.contains(event.mouseButton.x, event.mouseButton.y)) {
    window_->close();
  }
}