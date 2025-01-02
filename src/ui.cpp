#include "ui.hpp"

UI::UI(std::shared_ptr<Player> player, std::shared_ptr<GameState> gameState,
       sf::Font font, std::shared_ptr<sf::RenderWindow> window)
    : player_(player), gameState_(gameState), font(font), window_(window) {
  sf::Font f;
  if (!f.loadFromFile("HackNerdFont-Regular.ttf")) {
    std::cerr << "Error loading font!" << std::endl;
  }

  font = f;
  // set active menu to be the build menu
  activeMenu_ = BUILD;
}

void UI::displayUI() {
  displayMap();
  displayEndRound();
  displayResources();
  displayMenu();
  displaySelected();
  displayInfoBox();
};

void UI::processMouseButtonPressed(sf::Event event) {
  if (endTurnArea.contains(event.mouseButton.x, event.mouseButton.y)) {
    std::cout << "End turn pressed" << std::endl;
    gameState_->nextTurn();
  }
  if (mapArea.contains(event.mouseButton.x, event.mouseButton.y)) {
    int xPos =
        (event.mouseButton.x - LEFT_MAP_OFFSET) / (TILE_SIZE + OUTLINE_SIZE);
    int yPos =
        (event.mouseButton.y - TOP_MAP_OFFSET) / (TILE_SIZE + OUTLINE_SIZE);
    processTileSelected(xPos, yPos);
  }
  if (menuArea.contains(event.mouseButton.x, event.mouseButton.y)) {
    std::cout << "Menu Area clicked " << menuArea.width << std::endl;
    processMenuSelected(event);
  }
  // std::cout << event.mouseButton.x << " " << event.mouseButton.y <<
  // std::endl;
};

// ==============
//
// UI ELEMENTS
//
// ==============

void UI::displayResources() {
  auto r = player_->getResources();

  std::ostringstream oss;
  oss << "Wood: " << r.wood;

  sf::RectangleShape wood_rectangle =
      createRect(140, TOP_MAP_OFFSET - 1, 1140, 0, DARK_GRAY, 2, LIGHT_GRAY);
  sf::Text wood_text = createTextForRect(wood_rectangle, font, oss.str());

  oss.str("");
  oss << "Food: " << r.food;

  sf::RectangleShape food_rectangle =
      createRect(140, TOP_MAP_OFFSET - 1, 1000, 0, DARK_GRAY, 2, LIGHT_GRAY);
  sf::Text food_text = createTextForRect(food_rectangle, font, oss.str());

  oss.str("");
  oss << "Gold: " << r.gold;

  sf::RectangleShape gold_rectangle =
      createRect(140, TOP_MAP_OFFSET - 1, 860, 0, DARK_GRAY, 2, LIGHT_GRAY);
  sf::Text gold_text = createTextForRect(gold_rectangle, font, oss.str());

  oss.str("");
  oss << "Stone: " << r.stone;

  sf::RectangleShape stone_rectangle =
      createRect(140, TOP_MAP_OFFSET - 1, 720, 0, DARK_GRAY, 2, LIGHT_GRAY);
  sf::Text stone_text = createTextForRect(stone_rectangle, font, oss.str());

  window_->draw(wood_rectangle);
  window_->draw(wood_text);
  window_->draw(food_rectangle);
  window_->draw(food_text);
  window_->draw(gold_rectangle);
  window_->draw(gold_text);
  window_->draw(stone_rectangle);
  window_->draw(stone_text);
};

void UI::displaySelected() {
  std::ostringstream oss;
  std::string label = "";
  if (selected_ != nullptr) {
    label = selected_->getLabel();
  }
  oss << "Selected: " << label;

  sf::RectangleShape rectangle = createRect(
      436, TOP_MAP_OFFSET - 1, LEFT_MAP_OFFSET + 2, 0, UI_BG, 2, LIGHT_GRAY);
  sf::Text text = createTextForRect(rectangle, font, oss.str(), 18,
                                    sf::Color::White, START, CENTER);

  window_->draw(rectangle);
  window_->draw(text);
}

void UI::displayEndRound() {
  sf::RectangleShape rectangle;
  sf::Text text;

  if (gameState_->getActivePlayerID() == player_->getID()) {
    std::string label = "End Turn";
    rectangle = createRect(200, 160, 1080, 863, DARK_GREEN, 2, LIGHT_GRAY);
    text = createTextForRect(rectangle, font, label, 24);
  } else {
    rectangle = createRect(200, 160, 1080, 863, LIGHT_GRAY, 2, LIGHT_GRAY);
    text = createTextForRect(rectangle, font, "WAITING", 24);
  }
  window_->draw(rectangle);
  window_->draw(text);
};

void UI::displayInfoBox() {
  sf::RectangleShape rectangle;
  sf::Text title;

  std::string name = "Nothing selected";
  std::string label = "Nothing selected";
  std::vector<std::string> content;
  if (selected_ != nullptr) {
    name = selected_->getName();
    label = selected_->getLabel();
    content = selected_->getInfo();
  }
  rectangle =
      createRect(LEFT_MAP_OFFSET - 2, 400, 0, 0, DARK_GRAY, 2, LIGHT_GRAY);
  title = createTextForRect(rectangle, font, label, 24, sf::Color::White,
                            ORIENTATION::START, ORIENTATION::START);
  window_->draw(rectangle);
  window_->draw(title);
  for (sf::Text text : createTextsForRect(rectangle, font, content, 24,
                                          sf::Color::White, 52, 12)) {
    window_->draw(text);
  }
}

void UI::displayMap() {
  for (auto tile : gameState_->getVisibleTiles(player_)) {
    // tile is of size 38 and 2*1mm outline on each side
    drawTile(tile, LEFT_MAP_OFFSET + tile.getX() * (TILE_SIZE + OUTLINE_SIZE),
             TOP_MAP_OFFSET + tile.getY() * (TILE_SIZE + OUTLINE_SIZE));
  }

  displayHighlightedtiles();

  for (auto building : gameState_->getVisibleBuildings(player_)) {
    drawBuilding(building,
                 LEFT_MAP_OFFSET + building.getX() * (TILE_SIZE + OUTLINE_SIZE),
                 TOP_MAP_OFFSET + building.getY() * (TILE_SIZE + OUTLINE_SIZE));
  }

  for (auto townhall : gameState_->getVisibleTownhalls(player_)) {
    drawTownhall(townhall,
                 LEFT_MAP_OFFSET + townhall.getX() * (TILE_SIZE + OUTLINE_SIZE),
                 TOP_MAP_OFFSET + townhall.getY() * (TILE_SIZE + OUTLINE_SIZE));
  }

  for (auto army : gameState_->getVisibleArmies(player_)) {
    drawArmy(army,
             LEFT_MAP_OFFSET + army.getLocationX() * (TILE_SIZE + OUTLINE_SIZE),
             TOP_MAP_OFFSET + army.getLocationY() * (TILE_SIZE + OUTLINE_SIZE));
  }

  displayArmyMovementRange();
}

void UI::displayHighlightedtiles() {
  for (auto tile : highlightedTiles_) {
    drawTile(*tile, LEFT_MAP_OFFSET + tile->getX() * (TILE_SIZE + OUTLINE_SIZE),
             TOP_MAP_OFFSET + tile->getY() * (TILE_SIZE + OUTLINE_SIZE),
             true  // sets it to be highlighted
    );
  }
}

void UI::displayMenu() {
  sf::RectangleShape menuContainer =
      createRect(WINDOW_WIDTH - LEFT_MAP_OFFSET - 200, 160, LEFT_MAP_OFFSET,
                 863, UI_BG, 2, LIGHT_GRAY);

  std::string menuItem0Text = "";
  std::string menuItem1Text = "";
  std::string menuItem2Text = "";
  std::string menuItem3Text = "";
  switch (activeMenu_) {
    case BUILD:
      menuItem0Text = "Wood cutter";
      menuItem1Text = "Farm";
      menuItem2Text = "Mine";
      menuItem3Text = "Market";
      break;

    case ARMY:
      menuItem0Text = "Colonize";
      menuItem1Text = "Add Unit";
      menuItem2Text = "";
      menuItem3Text = "";
      break;

    case TOWNHALL:
      menuItem0Text = "Add " + getArmyTypeName(INFANTRY);
      menuItem1Text = "Add " + getArmyTypeName(CAVALRY);
      menuItem2Text = "Add " + getArmyTypeName(ARTILLERY);
      menuItem3Text = "Add " + getArmyTypeName(MARINE);
      break;

    default:
      break;
  }

  sf::RectangleShape menuItem0 =
      createRect(200, 160, (int)LEFT_MAP_OFFSET, 863, UI_BG, 1, LIGHT_GRAY);
  sf::RectangleShape menuItem1 = createRect(
      200, 160, (int)LEFT_MAP_OFFSET + 200, 863, DARK_GRAY, 1, LIGHT_GRAY);
  sf::RectangleShape menuItem2 = createRect(
      200, 160, (int)LEFT_MAP_OFFSET + 400, 863, UI_BG, 1, LIGHT_GRAY);
  sf::RectangleShape menuItem3 = createRect(
      200, 160, (int)LEFT_MAP_OFFSET + 600, 863, DARK_GRAY, 1, LIGHT_GRAY);

  sf::Text text0 = createTextForRect(menuItem0, font, menuItem0Text);
  sf::Text text1 = createTextForRect(menuItem1, font, menuItem1Text);
  sf::Text text2 = createTextForRect(menuItem2, font, menuItem2Text);
  sf::Text text3 = createTextForRect(menuItem3, font, menuItem3Text);

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

// ==============
//
// GAME PIECES
//
// ==============

void UI::drawTile(const Tile& tile, int xPos, int yPos, bool isHighlighted) {
  sf::RectangleShape rectangle(sf::Vector2f(38.f, 38.f));
  rectangle.setPosition(xPos, yPos);

  if (tile.getOwner() != nullptr) {
    rectangle.setFillColor(tile.getOwner()->getColor());
  } else {
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
      default:
        rectangle.setFillColor(sf::Color(155, 255, 155));
        break;
    }
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
};

void UI::drawBuilding(const Building& building, int xPos, int yPos) {
  sf::ConvexShape buildingShape;
  buildingShape.setPointCount(4);
  buildingShape.setPoint(0, sf::Vector2f(0.f, -TILE_SIZE / 2));
  buildingShape.setPoint(1, sf::Vector2f(TILE_SIZE / 2, 0.f));
  buildingShape.setPoint(2, sf::Vector2f(0.f, TILE_SIZE / 2));
  buildingShape.setPoint(3, sf::Vector2f(-TILE_SIZE / 2, 0.f));

  buildingShape.setFillColor(sf::Color::Red);

  buildingShape.setOrigin(TILE_SIZE / 2, TILE_SIZE / 2);

  buildingShape.setPosition(xPos + TILE_SIZE, yPos + TILE_SIZE);

  window_->draw(buildingShape);
}

void UI::drawTownhall(const TownHall& townhall, int xPos, int yPos) {
  const float radius = 19.f;  // Half the tile size minus a bit for a border
  const int sides = 6;

  sf::ConvexShape townhallShape;
  townhallShape.setPointCount(sides);

  for (int i = 0; i < sides; ++i) {
    float angle = 2 * M_PI * i /
                  sides;  // Calculate angle at which the point will be placed
    float x = std::cos(angle) * radius;
    float y = std::sin(angle) * radius;

    townhallShape.setPoint(
        i, sf::Vector2f(
               x + radius,
               y + radius));  // Position points relative to hexagon's center
  }
  townhallShape.setPosition(xPos, yPos);

  // Coloring le townhall (no outline color)
  townhallShape.setFillColor(
      sf::Color(255, 160, 0));  // (TODO:for now temp orange)Check tile we want
                                // to draw on for owner, set to owner color

  window_->draw(townhallShape);
}

void UI::drawArmy(const Army& army, int xPos, int yPos) {
  sf::Text text;

  text.setFont(font);
  text.setString(army.getName()[0] + std::to_string(army.getUnitCount()));

  text.setCharacterSize(18);

  text.setFillColor(army.getHasCompletedTurn() ? sf::Color::Black
                                               : army.getOwner()->getColor());
  text.setOutlineThickness(2);
  text.setOutlineColor(sf::Color::White);

  sf::FloatRect bounds = text.getLocalBounds();
  float offsetX = (TILE_SIZE - bounds.width) / 2;
  float offsetY = (TILE_SIZE - bounds.height) / 2;
  text.setPosition(xPos + offsetX, yPos + offsetY);

  window_->draw(text);
}

void UI::displayArmyMovementRange() {
  if (selected_) {
    Army* selectedArmy = dynamic_cast<Army*>(selected_);
    if (selectedArmy) {
      std::shared_ptr<Player> owner = selectedArmy->getOwner();
      if (owner->getID() == gameState_->getActivePlayerID()) {
        std::vector<std::pair<int, int>> movementRange =
            gameState_->getArmyMovementRange(*selectedArmy);
        auto it = movementRange.begin();
        while (it != movementRange.end()) {
          int xPos = LEFT_MAP_OFFSET + it->first * (TILE_SIZE + OUTLINE_SIZE);
          int yPos = TOP_MAP_OFFSET + it->second * (TILE_SIZE + OUTLINE_SIZE);
          drawArmyMovementSuggestionMarker(*selectedArmy, xPos, yPos);
          it++;
        }
      }
    }
  }
}

void UI::drawArmyMovementSuggestionMarker(const Army& army, int xPos,
                                          int yPos) {
  float radius = TILE_SIZE / 4;
  sf::CircleShape circle(radius);
  circle.setFillColor(sf::Color::Magenta);
  float offsetX = (TILE_SIZE - radius * 2) / 2;
  float offsetY = (TILE_SIZE - radius * 2) / 2;
  circle.setPosition(xPos + offsetX, yPos + offsetY);
  window_->draw(circle);
}

// ==============
//
// HELPERS
//
// ==============

void UI::addHighlightedTile(int xPos, int yPos) {
  Tile& tile = gameState_->getTile(xPos, yPos);
  highlightedTiles_.push_back(&tile);
}

void UI::removeHighlightedTile(int xPos, int yPos) {
  highlightedTiles_.pop_back();
}

void UI::processMenuSelected(sf::Event event) {
  // Menu is split into 4 parts, 0 indexed
  int menuItem = (event.mouseButton.x - menuArea.getPosition().x) / 200;

  if (activeMenu_ == BUILD) {
    BuildingBlueprint* selectedBuilding = new BuildingBlueprint;
    switch (menuItem) {
      case 0:
        selectedBuilding->setType(WOOD_CUTTER);
        break;

      case 1:
        selectedBuilding->setType(FARM);
        break;

      case 2:
        selectedBuilding->setType(MINE);
        break;

      case 3:
        selectedBuilding->setType(MARKET);
        break;

      default:
        // If a type couldn't be found don't change selected
        return;
        break;
    }
    resetSelected();
    selected_ = selectedBuilding;

  } else if (activeMenu_ == ARMY) {
    Army* army = dynamic_cast<Army*>(selected_);
    switch (menuItem) {
      case 0:
        std::cout << "Colonize selected" << std::endl;
        gameState_->colonize(*army);
        resetSelected();
        activeMenu_ = Menu::BUILD;
        break;

      case 1:
        std::cout << "Add unit selected" << std::endl;
        gameState_->incrementArmyUnit(*army);
        // To do: cost
        break;

      case 2:
        std::cout << "Nothing" << std::endl;
        break;

      case 3:
        std::cout << "Nothing" << std::endl;
        break;

      default:
        break;
    }

  } else if (activeMenu_ == TOWNHALL) {
    switch (menuItem) {
      case 0:
        std::cout << "Add " << getArmyTypeName(INFANTRY) << " selected"
                  << std::endl;
        gameState_->placeSoldiers(player_, INFANTRY);
        // activeMenu_ = Menu::BUILD;
        break;

      case 1:
        std::cout << "Add " << getArmyTypeName(CAVALRY) << " selected"
                  << std::endl;
        gameState_->placeSoldiers(player_, CAVALRY);
        break;

      case 2:
        std::cout << "Add " << getArmyTypeName(ARTILLERY) << " selected"
                  << std::endl;
        gameState_->placeSoldiers(player_, ARTILLERY);
        break;

      case 3:
        std::cout << "Add " << getArmyTypeName(MARINE) << " selected"
                  << std::endl;
        gameState_->placeSoldiers(player_, MARINE);
        break;

      default:
        break;
    }
  }
}

void UI::processTileSelected(int xPos, int yPos) {
  auto visibleXY = gameState_->getVisibleXY(player_);
  if (std::find(visibleXY.begin(), visibleXY.end(),
                std::make_pair(xPos, yPos)) == visibleXY.end()) {
    std::cout << "Tile (" << xPos << ", " << yPos << ") is not visible"
              << std::endl;
    return;
  }
  std::cout << "Tile (" << xPos << ", " << yPos << ") selected" << std::endl;
  // here should be the logic that checks if there is an army on the tile and
  // what is selected beforehand etc..
  if (!highlightedTiles_.empty())
    removeHighlightedTile(xPos,
                          yPos);  // ghetto implementation to remove the
                                  // highlight of a previously selected tile
  addHighlightedTile(xPos, yPos);

  BuildingBlueprint* selectedBuildingBlueprint =
      dynamic_cast<BuildingBlueprint*>(selected_);
  Army* selectedArmy = dynamic_cast<Army*>(selected_);

  if (selectedArmy) {
    bool isOwn =
        selectedArmy->getOwner()->getID() == gameState_->getActivePlayerID();
    bool isActionable = isOwn && !selectedArmy->getHasCompletedTurn();

    if (isActionable) {
      bool isMovable =
          gameState_->isArmyWithinMovementRange(*selectedArmy, xPos, yPos);
      if (isMovable) {
        gameState_->moveArmy(*selectedArmy, xPos, yPos);
        resetSelected();
        activeMenu_ = Menu::BUILD;
        return;
      } else {
        int x = selectedArmy->getLocationX();
        int y = selectedArmy->getLocationY();
        if (x == xPos && y == yPos) {
          resetSelected();
          activeMenu_ = Menu::BUILD;
          return;
        }
      }
    }
  }

  if (gameState_->tileHasArmy(xPos, yPos)) {
    if (selectedBuildingBlueprint) delete (selected_);
    Army& army = gameState_->getArmyByLocation(xPos, yPos);
    selected_ = &army;
    if (!army.getHasCompletedTurn()) {
      if (army.getOwner()->getID() == gameState_->getActivePlayerID())
        activeMenu_ = Menu::ARMY;
    }
  } else if (gameState_->tileHasTownhall(xPos, yPos)) {
    activeMenu_ = Menu::TOWNHALL;
    TownHall& Townhall = gameState_->getTownhallByLocation(xPos, yPos);
    selected_ = &Townhall;
  } else if (selectedBuildingBlueprint) {
    // check if user has enough resources to place a building
    Resources cost = selectedBuildingBlueprint->getResourceCost();
    const Resources& currentResources = player_->getResources();

    bool hasEnoughResources = currentResources.food >= cost.food &&
                              currentResources.wood >= cost.wood &&
                              currentResources.gold >= cost.gold &&
                              currentResources.stone >= cost.stone;

    if (!hasEnoughResources) {
      std::cout << "Cannot place building: Not enough resources." << std::endl;
      resetSelected();
      activeMenu_ = Menu::BUILD;
      return;
    }

    // check if tile is valid for building placement
    if (gameState_->canPlaceBuilding(xPos, yPos, player_->getID())) {
      Building newBuilding = Building(
          selectedBuildingBlueprint->getType(),
          selectedBuildingBlueprint->getResourceCost(),
          selectedBuildingBlueprint->getResourceGain(), player_, xPos, yPos);

      gameState_->addBuilding(newBuilding);

      std::cout << "Building placed on tile (" << xPos << ", " << yPos << ")"
                << std::endl;

      // deduct resources from the player
      Resources cost = selectedBuildingBlueprint->getResourceCost();
      Resources negatedCost(-cost.food, -cost.wood, -cost.gold, -cost.stone);
      player_->modifyResources(negatedCost);

      // cleanup and reset selection
      resetSelected();
      activeMenu_ = Menu::BUILD;

    } else {
      // if the tile is invalid, provides feedback
      Tile& tile = gameState_->getTile(xPos, yPos);

      if (tile.getOwner() == nullptr ||
          tile.getOwner()->getID() != player_->getID()) {
        std::cout << "Can't put building: Tile (" << xPos << ", " << yPos
                  << ") is not owned by you." << std::endl;
      } else if (gameState_->tileHasBuilding(xPos, yPos)) {
        std::cout << "Can't put building: Tile (" << xPos << ", " << yPos
                  << ") is already occupied." << std::endl;
      } else {
        std::cout << "Can't put building: Invalid tile (" << xPos << ", "
                  << yPos << ")." << std::endl;
      }
    }
  } else {
    resetSelected();
    activeMenu_ = Menu::BUILD;
  }
}

Selectable* UI::getSelected() const { return selected_; }

void UI::setSelected(Selectable* selectable) { selected_ = selectable; }

void UI::resetSelected() {
  BuildingBlueprint* selectedBuildingBlueprint =
      dynamic_cast<BuildingBlueprint*>(selected_);
  // if this evaluates to true, a BuildingBlueprint was selected before and thus
  // it needs to be deallocated this is the only time we need to deallocate, all
  // other pointers point to stack allocated memory
  if (selectedBuildingBlueprint) delete selected_;

  selected_ = nullptr;
}