#include "mainmenu.hpp"

MainMenu::MainMenu(UIManager& uiManager, sf::Font font,
                   std::shared_ptr<sf::RenderWindow> window)
    : uiManager_(uiManager), font_(font), window_(window) {}

void MainMenu::displayUI() {
    // Background and Title
    sf::RectangleShape background = createRect(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, sf::Color(0, 100, 125));
    sf::RectangleShape titleBox = createRect(250, 100, WINDOW_WIDTH/2-250/2, WINDOW_HEIGHT/2-100/2-200, sf::Color::Black);
    sf::Text titleText = createTextForRect(titleBox, font_, "Strategy-8", 26);

    window_->draw(background); 
    window_->draw(titleText); 

    // Start button
    sf::RectangleShape startButton = createRect(250, 100, WINDOW_WIDTH/2-250/2, WINDOW_HEIGHT/2-100/2-50, sf::Color::Green);
    sf::Text startText = createTextForRect(startButton, font_, "Start");

  window_->draw(startButton);
  window_->draw(startText);

  // Map editor button
  sf::RectangleShape mapeditorButton =
      createRect(250, 100, WINDOW_WIDTH / 2 - 250 / 2,
                 WINDOW_HEIGHT / 2 - 100 / 2 + 50, sf::Color::Blue);
  sf::Text mapeditorText =
      createTextForRect(mapeditorButton, font_, "Map Editor");

  window_->draw(mapeditorButton);
  window_->draw(mapeditorText);
}

void MainMenu::processMouseButtonPressed(sf::Event event) {
  if (startArea.contains(event.mouseButton.x, event.mouseButton.y)) {
    std::cout << "Start button pressed" << std::endl;
    uiManager_.setState(GAME);
  } else if (mapeditorArea.contains(event.mouseButton.x, event.mouseButton.y)) {
    std::cout << "Map editor button pressed" << std::endl;
    uiManager_.setState(MAPEDITOR);
  }
}