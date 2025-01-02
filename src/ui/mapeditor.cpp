#include "mapeditor.hpp"

std::string readFileToStringMapEd(const std::string& filename);
void loadMapFromString(const std::string str, int num_rows);

MapEditor::MapEditor(UIManager& uiManager, sf::Font font,
                     std::shared_ptr<sf::RenderWindow> window)
    : uiManager_(uiManager), font_(font), window_(window) {
  std::string filename = "maps/grey.txt";
  std::string fileContent = readFileToStringMapEd(filename);
  loadMapFromString(fileContent, NUM_OF_ROWS);
}

/////////////////
/// UI ELEMENTS///
/////////////////

void MapEditor::drawTile(const Tile& tile, int xPos, int yPos,
                         bool isHighlighted) {
  sf::RectangleShape rectangle(sf::Vector2f(38.f, 38.f));
  rectangle.setPosition(xPos, yPos);
  switch (tile.getType()) {
    case GRASS:
      rectangle.setFillColor(sf::Color(155, 255, 155));
      break;
    case WATER:
      rectangle.setFillColor(sf::Color(45, 110, 210));
      break;
    case ROCK:
      rectangle.setFillColor(sf::Color(210, 210, 210));
      break;
    case PLACEHOLDER:
      rectangle.setFillColor(sf::Color(128, 128, 128));
      break;
    default:
      rectangle.setFillColor(sf::Color(155, 255, 155));
      break;
  }

  rectangle.setOutlineThickness(1);
  rectangle.setOutlineColor(sf::Color(200, 200, 200));

  if (isHighlighted) {
    // make the tile brighter
    int brightnessIncrease = 50;
    sf::Color newColor = rectangle.getFillColor();
    newColor.r = std::min(newColor.r + brightnessIncrease, 255);
    newColor.g = std::min(newColor.g + brightnessIncrease, 255);
    newColor.b = std::min(newColor.b + brightnessIncrease, 255);
    rectangle.setFillColor(newColor);
  }

  window_->draw(rectangle);
}

void MapEditor::displayUI() {
  displayMap();
  displayMenu();
  displaySave();
}

void MapEditor::displayMap() {
  for (auto tile : placeHolderMap) {
    // tile is of size 38 and 2*1mm outline on each side
    drawTile(tile, LEFT_MAP_OFFSET + tile.getX() * (TILE_SIZE + OUTLINE_SIZE),
             TOP_MAP_OFFSET + tile.getY() * (TILE_SIZE + OUTLINE_SIZE));
  }

  displayHighlightedtiles();
}

void MapEditor::displaySave() {
  sf::RectangleShape rectangle;
  sf::Text text;

  std::string label = "Save Map";
  rectangle = createRect(200, 160, 1080, 863, DARK_GREEN, 2, LIGHT_GRAY);
  text = createTextForRect(rectangle, font_, label, 24);

  window_->draw(rectangle);
  window_->draw(text);
}

void MapEditor::displayMenu() {
  sf::RectangleShape menuContainer =
      createRect(WINDOW_WIDTH - LEFT_MAP_OFFSET - 200, 160, LEFT_MAP_OFFSET,
                 863, UI_BG, 2, LIGHT_GRAY);

  std::string menuItem0Text = "GRASS";
  std::string menuItem1Text = "ROCK";
  std::string menuItem2Text = "WATER";
  std::string menuItem3Text = "NONE";

  sf::RectangleShape menuItem0 =
      createRect(200, 160, (int)LEFT_MAP_OFFSET, 863, UI_BG, 1, LIGHT_GRAY);
  sf::RectangleShape menuItem1 = createRect(
      200, 160, (int)LEFT_MAP_OFFSET + 200, 863, DARK_GRAY, 1, LIGHT_GRAY);
  sf::RectangleShape menuItem2 = createRect(
      200, 160, (int)LEFT_MAP_OFFSET + 400, 863, UI_BG, 1, LIGHT_GRAY);
  sf::RectangleShape menuItem3 = createRect(
      200, 160, (int)LEFT_MAP_OFFSET + 600, 863, DARK_GRAY, 1, LIGHT_GRAY);

  sf::Text text0 = createTextForRect(menuItem0, font_, menuItem0Text);
  sf::Text text1 = createTextForRect(menuItem1, font_, menuItem1Text);
  sf::Text text2 = createTextForRect(menuItem2, font_, menuItem2Text);
  sf::Text text3 = createTextForRect(menuItem3, font_, menuItem3Text);

  window_->draw(menuContainer);

  window_->draw(menuItem0);
  window_->draw(menuItem1);
  window_->draw(menuItem2);
  window_->draw(menuItem3);

  window_->draw(text0);
  window_->draw(text1);
  window_->draw(text2);
  window_->draw(text3);
}

void MapEditor::displayHighlightedtiles() {
  for (auto tile : highlightedTiles_) {
    drawTile(tile, LEFT_MAP_OFFSET + tile.getX() * (TILE_SIZE + OUTLINE_SIZE),
             TOP_MAP_OFFSET + tile.getY() * (TILE_SIZE + OUTLINE_SIZE),
             true  // sets it to be highlighted
    );
  }
}

void MapEditor::processMenuSelected(sf::Event event) {
  // Menu is split into 4 parts, 0 indexed
  int menuItem = (event.mouseButton.x - menuArea.getPosition().x) / 200;
  switch (menuItem) {
    case 0:
      selectedType_ = GRASS;
      break;

    case 1:
      selectedType_ = ROCK;
      break;

    case 2:
      selectedType_ = WATER;
      break;

    case 3:
      selectedType_ = PLACEHOLDER;
      break;

    default:
      // If a type couldn't be found don't change selected
      return;
      break;
  }
}

void MapEditor::processTileSelected(int xPos, int yPos) {
  getTile(xPos, yPos).setType(selectedType_);
  if (!highlightedTiles_.empty())
    highlightedTiles_.pop_back();  // ghetto implementation to remove the
                                   // highlight of a previously selected tile
  addHighlightedTile(xPos, yPos);
}

void MapEditor::processMouseButtonPressed(sf::Event event) {
  if (endTurnArea.contains(event.mouseButton.x, event.mouseButton.y)) {
    saveMapToFile("maps/saved_map.txt");
  }
  if (mapArea.contains(event.mouseButton.x, event.mouseButton.y)) {
    int xPos =
        (event.mouseButton.x - LEFT_MAP_OFFSET) / (TILE_SIZE + OUTLINE_SIZE);
    int yPos =
        (event.mouseButton.y - TOP_MAP_OFFSET) / (TILE_SIZE + OUTLINE_SIZE);
    processTileSelected(xPos, yPos);
  }
  if (menuArea.contains(event.mouseButton.x, event.mouseButton.y)) {
    processMenuSelected(event);
  }
}

/////////////////
/////HELPERS/////
/////////////////

// Works but saves map into build/maps/....
void MapEditor::saveMapToFile(const std::string& filename) const {
  std::ofstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Could not open the file for writing: " << filename
              << std::endl;
    return;
  }

  for (int i = 0; i < NUM_OF_ROWS * NUM_OF_COLS; i++) {
    const Tile& tile = placeHolderMap[i];
    switch (tile.getType()) {
      case GRASS:
        file << 'G';
        break;
      case WATER:
        file << 'W';
        break;
      case ROCK:
        file << 'R';
        break;
      case PLACEHOLDER:
      default:
        file << 'N';
        break;
    }
    if (i > 0 && ((i + 1) % NUM_OF_ROWS == 0)) {
      file << '\n';  // Move to the next row
    }
  }

  file.close();
  std::cout << "Map saved to " << filename << std::endl;
}

void MapEditor::addHighlightedTile(int xPos, int yPos) {
  Tile& tile = getTile(xPos, yPos);
  highlightedTiles_.push_back(tile);
}

bool MapEditor::areCoordinatesValid(int x, int y) const {
  int idx = x * NUM_OF_ROWS + y;
  return idx >= 0 && idx < placeHolderMap.size() && y < NUM_OF_ROWS && x >= 0 &&
         y >= 0;
}

Tile& MapEditor::getTile(int xPos, int yPos) {
  int idx = xPos * NUM_OF_ROWS + yPos;

  if (areCoordinatesValid(xPos, yPos)) {
    return placeHolderMap[idx];
  } else {
    throw std::out_of_range("Invalid tile coordinates");
  }
}

void MapEditor::loadMapFromString(const std::string str, int num_rows) {
  for (int i = 0; i < str.size(); i++) {
    switch (str[i]) {
      case 'G':
        placeHolderMap.push_back(Tile(i / num_rows, i % num_rows));
        break;
      case 'W':
        placeHolderMap.push_back(Tile(i / num_rows, i % num_rows, WATER));
        break;
      case 'R':
        placeHolderMap.push_back(Tile(i / num_rows, i % num_rows, ROCK));
        break;
      case 'N':
        placeHolderMap.push_back(Tile(i / num_rows, i % num_rows, PLACEHOLDER));
        break;
      default:
        placeHolderMap.push_back(Tile(i / num_rows, i % num_rows));
        break;
    }
  }
}

std::string readFileToStringMapEd(const std::string& filename) {
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