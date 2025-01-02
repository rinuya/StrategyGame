#include "gamestate.hpp"

#include <cmath>

GameState::GameState(std::shared_ptr<sf::RenderWindow> window, int turn)
    : window_(window), turn_(turn) {}

void GameState::tick() { players_[turn_ % players_.size()]->doTurn(); }

void GameState::addPlayer(std::shared_ptr<Player> player) {
  players_.push_back(std::move(player));
}

int GameState::getActivePlayerID() { return activePlayerID_; }

void GameState::addBuilding(const Building& building) {
  buildings_.push_back(building);
}

void GameState::addTownhall(const TownHall& townhall) {
  townhalls_.push_back(townhall);
}

void GameState::addArmy(ArmyType type, int x, int y,
                        std::shared_ptr<Player> owner, int unitCount) {
  Army army(type, std::make_pair(x, y), owner, unitCount);
  armies_.push_back(army);
  // std::cout << "added new army at " << x << " " << y << " for " << owner <<
  // std::endl;
}

void GameState::setActivePlayerID(int playerID) { activePlayerID_ = playerID; }

void GameState::loadMap(const std::vector<Tile>& tiles, int num_rows) {
  num_rows_ = num_rows;
  map_ = tiles;
}

void GameState::loadMapFromString(const std::string str, int num_rows) {
  std::vector<Tile> tiles;
  for (int i = 0; i < str.size(); i++) {
    // std::cout << str[i] << std::endl;
    switch (str[i]) {
      case 'G':
        tiles.push_back(Tile(i / num_rows, i % num_rows));
        break;
      case 'W':
        tiles.push_back(Tile(i / num_rows, i % num_rows, WATER));
        break;
      case 'R':
        tiles.push_back(Tile(i / num_rows, i % num_rows, ROCK));
        break;
      default:
        tiles.push_back(Tile(i / num_rows, i % num_rows));
        break;
    }
  }

  loadMap(tiles, num_rows);
}

Tile& GameState::getTile(int xPos, int yPos) {
  int idx = xPos * num_rows_ + yPos;

  if (areCoordinatesValid(xPos, yPos)) {
    return map_[idx];
  } else {
    throw std::out_of_range("Invalid tile coordinates");
  }
}

std::vector<Tile> GameState::getClaimedTiles(
    const std::shared_ptr<Player> player) {
  std::vector<Tile> tiles = std::vector<Tile>();

  for (auto& tile : map_) {
    if (tile.getOwner() == player) {
      tiles.push_back(tile);
    }
  }

  return tiles;
}

void GameState::claimTile(int x, int y, const std::shared_ptr<Player>& player) {
  Tile& tile = getTile(x, y);
  tile.setOwner(player);
}

void GameState::nextTurn() {
  int pastPlayerNr = turn_ % players_.size();
  for (auto building : buildings_) {
    if (building.getOwner() == nullptr)
      std::cout << "Building owner of building at " << building.getX() << ", "
                << building.getY() << " is nullptr" << std::endl;

    if (building.getOwner()->getID() == activePlayerID_) {
      players_[pastPlayerNr]->modifyResources(building.nextTurn());
    }
  }
  /*
  for (auto army : armies_) {
      if(army.getOwner()->getID() == activePlayerID_){
          players_[pastPlayerNr]->modifyResources(Resources(-army.getMaintenanceCost(),0,0,0));
      }
  }
  */
  // Add 1 resource of everything every turn so player cannot softlock itself
  players_[pastPlayerNr]->modifyResources(Resources(1, 1, 1, 1));
  turn_++;

  // Goto next player
  int currentPlayerNr = turn_ % players_.size();
  setActivePlayerID(players_[currentPlayerNr]->getID());

  auto it = armies_.begin();
  while (it != armies_.end()) {
    it->setHasCompletedTurn(false);
    it++;
  }
  // Invokes AI players turn. If it's a regular player, this does nothing
  players_[currentPlayerNr]->doTurn();
}

std::vector<std::pair<int, int>> GameState::getVisibleXY(
    std::shared_ptr<Player> player) {
  std::vector<std::pair<int, int>> xyPairs = std::vector<std::pair<int, int>>();
  std::vector<Tile> tiles = std::vector<Tile>();

  for (auto army : armies_) {
    if (army.getOwner() != player) continue;
    int x = army.getLocationX();
    int y = army.getLocationY();
    tiles.push_back(getTile(x, y));
    for (auto tile : getNeighbourTiles(x, y, 1)) {
      // std::cout << "Adding army visible tile " << tile.getX() << " " <<
      // tile.getY() << std::endl;
      tiles.push_back(tile);
    }
  }
  for (auto building : buildings_) {
    if (building.getOwner() != player) continue;
    int x = building.getX();
    int y = building.getY();
    tiles.push_back(getTile(x, y));
    for (auto tile : getNeighbourTiles(x, y, 1)) {
      tiles.push_back(tile);
    }
  }
  for (auto townhall : townhalls_) {
    if (townhall.getOwner() != player) continue;
    int x = townhall.getX();
    int y = townhall.getY();
    tiles.push_back(getTile(x, y));
    for (auto tile : getNeighbourTiles(x, y, 1)) {
      tiles.push_back(tile);
    }
  }
  for (auto mapTile : map_) {
    if (mapTile.getOwner() != player) continue;
    int x = mapTile.getX();
    int y = mapTile.getY();
    tiles.push_back(getTile(x, y));
    for (auto tile : getNeighbourTiles(x, y, 1)) {
      tiles.push_back(tile);
    }
  }

  for (auto tile : tiles) {
    xyPairs.push_back(tile.getXY());
  }
  return xyPairs;
}

std::vector<Tile> GameState::getVisibleTiles(std::shared_ptr<Player> player) {
  std::vector<Tile> tiles = std::vector<Tile>();
  std::vector<std::pair<int, int>> visibleXY = getVisibleXY(player);
  std::copy_if(map_.begin(), map_.end(), std::back_inserter(tiles),
               [visibleXY](Tile tile) {
                 return std::find(visibleXY.begin(), visibleXY.end(),
                                  tile.getXY()) != visibleXY.end();
               });
  return tiles;
};

std::vector<Building> GameState::getVisibleBuildings(
    std::shared_ptr<Player> player) {
  std::vector<Building> buildings = std::vector<Building>();
  std::vector<std::pair<int, int>> visibleXY = getVisibleXY(player);
  std::copy_if(buildings_.begin(), buildings_.end(),
               std::back_inserter(buildings), [visibleXY](Building building) {
                 return std::find(visibleXY.begin(), visibleXY.end(),
                                  building.getXY()) != visibleXY.end();
               });
  return buildings;
}

std::vector<TownHall> GameState::getVisibleTownhalls(
    std::shared_ptr<Player> player) {
  std::vector<TownHall> townhalls = std::vector<TownHall>();
  std::vector<std::pair<int, int>> visibleXY = getVisibleXY(player);
  std::copy_if(townhalls_.begin(), townhalls_.end(),
               std::back_inserter(townhalls), [visibleXY](TownHall townhall) {
                 return std::find(visibleXY.begin(), visibleXY.end(),
                                  townhall.getXY()) != visibleXY.end();
               });
  return townhalls;
}

std::vector<Army> GameState::getVisibleArmies(std::shared_ptr<Player> player) {
  std::vector<Army> armies = std::vector<Army>();
  std::vector<std::pair<int, int>> visibleXY = getVisibleXY(player);
  std::copy_if(armies_.begin(), armies_.end(), std::back_inserter(armies),
               [visibleXY](Army army) {
                 return std::find(visibleXY.begin(), visibleXY.end(),
                                  army.getLocation()) != visibleXY.end();
               });
  return armies;
}

std::vector<Tile> GameState::getNeighbourTiles(int xPos, int yPos, int radius) {
  std::vector<Tile> temp;

  if (!areCoordinatesValid(xPos, yPos))
    throw std::out_of_range("Invalid tile coordinates");
  for (int xi = xPos - radius; xi <= xPos + radius; xi++) {
    for (int yi = yPos - radius; yi <= yPos + radius; yi++) {
      if (areCoordinatesValid(xi, yi) && !(xi == xPos && yi == yPos)) {
        temp.push_back(getTile(xi, yi));
      }
    }
  }

  return temp;
}

std::vector<Tile> GameState::getNeighbourGrassTilesNoDiagonal(int xPos,
                                                              int yPos,
                                                              int radius) {
  std::vector<Tile> temp;

  if (!areCoordinatesValid(xPos, yPos))
    throw std::out_of_range("Invalid tile coordinates");

  for (int xi = xPos - radius; xi <= xPos + radius; xi++) {
    for (int yi = yPos - radius; yi <= yPos + radius; yi++) {
      // Only include tiles that are horizontally or vertically adjacent within
      // the radius
      if ((xi == xPos || yi == yPos) && !(xi == xPos && yi == yPos) &&
          areCoordinatesValid(xi, yi) && getTile(xi, yi).getType() == GRASS) {
        temp.push_back(getTile(xi, yi));
      }
    }
  }

  return temp;
}

// Yoinked Junya's implementation
std::vector<TownHall>::iterator GameState::findTownhallByLocation(int x,
                                                                  int y) {
  return std::find_if(
      townhalls_.begin(), townhalls_.end(),
      [x, y](const TownHall& a) { return a.getX() == x && a.getY() == y; });
}

TownHall& GameState::getTownhallByLocation(int x, int y) {
  if (!areCoordinatesValid(x, y))
    throw std::out_of_range("Invalid tile coordinates");

  auto it = townhalls_.begin();
  while (it != townhalls_.end()) {
    if (it->getX() == x && it->getY() == y) return *it;
    it++;
  }
  throw std::invalid_argument("No townhall is located on the tile");
}

std::shared_ptr<Player> GameState::getTownhallOwner(int x, int y) {
  auto it = std::find_if(
      townhalls_.begin(), townhalls_.end(),
      [x, y](const TownHall& a) { return a.getX() == x && a.getY() == y; });

  if (it != townhalls_.end()) {
    return it->getOwner();
  }
  return nullptr;
}

bool GameState::tileHasTownhall(int x, int y) {
  return findTownhallByLocation(x, y) != townhalls_.end() &&
         getTownhallOwner(x, y)->getID() == activePlayerID_;
}

void GameState::placeSoldiers(std::shared_ptr<Player> player,
                              ArmyType armyType) {
  std::pair<int, int> TWCoords;
  TownHall currentTW =
      townhalls_[0];  // bad temp I know but it be like that sometimes
  for (auto Townhall : townhalls_) {
    if (Townhall.getOwner()->getID() == player->getID()) {
      TWCoords = Townhall.getPosition();
    }
  }

  // Check if player has enough resources to place an army
  if (!currentTW.canRecruitSoldier(player->getResources(), armyType)) return;
  Resources cost = getArmyDeploymentCost(armyType);
  Resources costNeg =
      Resources(-cost.food, -cost.wood, -cost.gold, -cost.stone);

  std::vector<Tile> TWNeigbours =
      getNeighbourTiles(TWCoords.first, TWCoords.second, 1);

  // Army merge logic
  for (auto Tile : TWNeigbours) {
    if (tileHasArmy(Tile.getX(), Tile.getY())) {
      Army& army = getArmyByLocation(Tile.getX(), Tile.getY());
      std::shared_ptr<Player> armyOwner = army.getOwner();
      if (armyOwner == player && army.getType() == armyType) {
        // Army merges
        army.incrementUnitCount();
        player->modifyResources(costNeg);
        break;
      }
    } else if (Tile.getType() == GRASS) {
      addArmy(armyType, Tile.getX(), Tile.getY(), player, 1);
      player->modifyResources(costNeg);
      break;
    }
  }
}

Army& GameState::getArmyByLocation(int x, int y) {
  if (!areCoordinatesValid(x, y))
    throw std::out_of_range("Invalid tile coordinates");

  auto it = armies_.begin();
  while (it != armies_.end()) {
    if (it->getLocationX() == x && it->getLocationY() == y) return *it;
    it++;
  }
  throw std::invalid_argument("No army is located on the tile");
}

std::vector<Army>::iterator GameState::findArmyByLocation(int x, int y) {
  return std::find_if(armies_.begin(), armies_.end(), [x, y](const Army& a) {
    // std::cout << a.getLocationX() << "==" << x << "&&" << a.getLocationY() <<
    // "==" << y << std::endl;
    return a.getLocationX() == x && a.getLocationY() == y;
  });
}

bool GameState::tileHasArmy(int x, int y) {
  return findArmyByLocation(x, y) != armies_.end();
}

std::vector<std::pair<int, int>> GameState::getArmyMovementRange(Army& army) {
  std::vector<std::pair<int, int>> locations;
  int speed = 1;
  int x0 = army.getLocationX();
  int y0 = army.getLocationY();

  if (army.getHasCompletedTurn()) return locations;

  for (int xOffset = -speed; xOffset <= speed; xOffset++) {
    int offsetRemainder = speed - abs(xOffset);
    for (int yOffset = -offsetRemainder; yOffset <= offsetRemainder;
         yOffset++) {
      int x = x0 + xOffset;
      int y = y0 + yOffset;
      if (isArmyWithinMovementRange(army, x, y)) {
        locations.push_back(std::make_pair(x, y));
      }
    }
  }

  return locations;
}

bool GameState::isArmyWithinMovementRange(Army& army, int x, int y) {
  int speed = 1;

  if (!areCoordinatesValid(x, y)) return false;

  int x0 = army.getLocationX();
  int y0 = army.getLocationY();

  if (x == x0 && y == y0) return false;

  int xOffset = x - x0;
  int yOffset = y - y0;
  if (abs(xOffset) + abs(yOffset) > speed) return false;

  Tile& tile = getTile(x, y);
  if (!army.canAdvance(tile.getType())) return false;

  std::shared_ptr<Player> armyOwner = army.getOwner();
  std::shared_ptr<Player> tileOwner = tile.getOwner();

  // Army cannot move from an enemy's tile to the same enemy's tile
  if (armyOwner != tileOwner && tileOwner &&
      getTile(x0, y0).getOwner() == tileOwner)
    return false;

  if (tileHasArmy(x, y)) {
    Army& armyOnTile = getArmyByLocation(x, y);
    if (armyOnTile.getOwner() == army.getOwner() &&
        armyOnTile.getType() != army.getType()) {
      return false;
    }
  }

  return true;
}

void GameState::moveArmy(Army& army, int x, int y) {
  if (!areCoordinatesValid(x, y))
    throw std::out_of_range("Invalid tile coordinates");

  std::shared_ptr<Player> armyOwner = army.getOwner();
  Tile& tile = getTile(x, y);
  std::shared_ptr<Player> tileOwner = tile.getOwner();
  TileType tileType = tile.getType();
  auto armyOnTileIt = findArmyByLocation(x, y);
  int x0 = army.getLocationX();
  int y0 = army.getLocationY();

  if (isArmyWithinMovementRange(army, x, y)) {
    if (armyOnTileIt == armies_.end()) {
      // Army moves
      army.setLocation(std::make_pair(x, y));
      army.setHasCompletedTurn(true);
    } else {
      std::shared_ptr<Player> otherArmyOwner = armyOnTileIt->getOwner();
      int ownUnitCount = army.getUnitCount();
      int otherUnitCount = armyOnTileIt->getUnitCount();
      if (otherArmyOwner == armyOwner) {
        // Army merges
        army.setLocation(std::make_pair(x, y));
        army.setUnitCount(ownUnitCount + otherUnitCount);
        army.setHasCompletedTurn(true);
        armies_.erase(armyOnTileIt);
      } else {
        // Combat
        int enemyCasualty = std::round(ownUnitCount * army.getAttack() /
                                       armyOnTileIt->getDefense() * .6);
        int ownCasualty =
            std::round(otherUnitCount * armyOnTileIt->getAttack() /
                       army.getDefense() * .7);
        ownUnitCount = std::max(ownUnitCount - ownCasualty, 0);
        otherUnitCount = std::max(otherUnitCount - enemyCasualty, 0);
        army.setUnitCount(ownUnitCount);
        armyOnTileIt->setUnitCount(otherUnitCount);

        bool isSelfAlive = ownUnitCount > 0;
        bool isEnemyAlive = otherUnitCount > 0;

        if (isSelfAlive && !isEnemyAlive) {
          // Victory
          army.setLocation(std::make_pair(x, y));
          army.setHasCompletedTurn(true);
          armies_.erase(armyOnTileIt);
        } else if (!isSelfAlive && isEnemyAlive) {
          // Defeat
          auto armyIt = findArmyByLocation(x0, y0);
          armies_.erase(armyIt);
        } else {
          // Draw
          if (!isSelfAlive && !isEnemyAlive) {
            armies_.erase(armyOnTileIt);

            auto armyIt = findArmyByLocation(x0, y0);
            armies_.erase(armyIt);
          } else {
            army.setHasCompletedTurn(true);
          }
        }
      }
    }
  }
}

void GameState::colonize(Army& army) {
  int x = army.getLocationX();
  int y = army.getLocationY();
  std::shared_ptr<Player> player = army.getOwner();

  Tile& tile = getTile(x, y);
  std::shared_ptr<Player> tileOwner = tile.getOwner();
  auto buildingIt = findBuildingByLocation(x, y);

  auto it = std::find_if(players_.begin(), players_.end(),
                         [&tileOwner](const std::shared_ptr<Player>& player) {
                           return player == tileOwner;
                         });
  if (it == players_.end()) {
    tile.setOwner(player);
  } else {
    std::cout << "If tileOwner == player return" << std::endl;
    if (tileOwner == player) return;
    auto townhallIt = findTownhallByLocation(x, y);
    if (townhallIt != townhalls_.end()) {
      std::cout << "If townhallowner == player return" << std::endl;
      std::shared_ptr<Player> townhallOwner = townhallIt->getOwner();
      if (townhallOwner == player) return;
      handleGameover(townhallOwner);
    } else {
      tile.setOwner(nullptr);
      if (buildingIt != buildings_.end()) {
        buildings_.erase(buildingIt);
      }
    }
  }
  army.setHasCompletedTurn(true);
}

std::vector<std::reference_wrapper<Army>> GameState::getPlayerArmies(
    std::shared_ptr<Player> player) {
  std::vector<std::reference_wrapper<Army>> playerArmies;
  for (Army& a : armies_) {  // Use reference here
    if (a.getOwner() == player) playerArmies.push_back(a);
  }
  return playerArmies;
}

bool GameState::areCoordinatesValid(int x, int y) const {
  int idx = x * num_rows_ + y;
  return idx >= 0 && idx < map_.size() && y < num_rows_ && x >= 0 && y >= 0;
}

std::vector<Building>::iterator GameState::findBuildingByLocation(int x,
                                                                  int y) {
  return std::find_if(
      buildings_.begin(), buildings_.end(),
      [x, y](const Building& b) { return b.getX() == x && b.getY() == y; });
}

bool GameState::tileHasBuilding(int x, int y) {
  return findBuildingByLocation(x, y) != buildings_.end();
}

bool GameState::canPlaceBuilding(int x, int y, int playerID) {
  if (!areCoordinatesValid(x, y)) {
    return false;
  }

  Tile& tile = getTile(x, y);

  if (tile.getOwner() == nullptr || tile.getOwner()->getID() != playerID) {
    return false;
  }

  if (tileHasBuilding(x, y)) {
    return false;
  }

  return true;
}

void GameState::handleGameover(std::shared_ptr<Player> player) {
  std::cout << "Player " << player->getID() << " lost" << std::endl;
  player->setIsAlive(false);

  auto tileIt = map_.begin();

  while (tileIt != map_.end()) {
    std::shared_ptr<Player> tileOwner = tileIt->getOwner();
    if (tileOwner && tileOwner->getID() == player->getID()) {
      tileIt->setOwner(nullptr);
    }
    tileIt++;
  }

  destroyArmiesByPlayer(player);
  destroyBuildingsByPlayer(player);
}

std::shared_ptr<Player> GameState::getWinner() const {
  auto it = players_.begin();
  std::shared_ptr<Player> winnerCandidate = nullptr;

  while (it != players_.end()) {
    if ((*it)->getIsAlive()) {
      if (winnerCandidate) return nullptr;
      winnerCandidate = *it;
    }
    it++;
  }

  return winnerCandidate;
}

void GameState::destroyArmiesByPlayer(std::shared_ptr<Player> player) {
  std::vector<Army> newArmies;
  std::copy_if(armies_.begin(), armies_.end(), std::back_inserter(newArmies),
               [player](Army& army) {
                 std::shared_ptr<Player> owner = army.getOwner();
                 return owner && owner->getID() == player->getID();
               });
  armies_ = newArmies;
}

void GameState::destroyBuildingsByPlayer(std::shared_ptr<Player> player) {
  std::vector<Building> newBuildings;
  std::copy_if(buildings_.begin(), buildings_.end(),
               std::back_inserter(newBuildings), [player](Building& building) {
                 std::shared_ptr<Player> owner = building.getOwner();
                 return owner && owner->getID() == player->getID();
               });
  buildings_ = newBuildings;
}

void GameState::incrementArmyUnit(Army& army) {
  std::shared_ptr<Player> player = army.getOwner();
  Resources resources = player->getResources();
  Resources cost = getArmyDeploymentCost(army.getType());

  if (resources.food < cost.food) return;
  if (resources.wood < cost.wood) return;
  if (resources.gold < cost.gold) return;
  if (resources.stone < cost.stone) return;

  Resources costNeg =
      Resources(-cost.food, -cost.wood, -cost.gold, -cost.stone);
  player->modifyResources(costNeg);
  army.incrementUnitCount();
}