#include "playerAI.hpp"

void PlayerAI::addSharedPtr(std::shared_ptr<PlayerAI> refToSelf) {
  self = refToSelf;
}

void PlayerAI::takeTurnActions() {
  currentActionStep_++;

  doBuildActions();
  doArmyActions();

  if (currentActionStep_ > maxSteps_) {
    // delay actions to see their impact
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    currentActionStep_ = 0;
    gameState_->nextTurn();
  }
};

// =============
//
// ARMY ACTIONS
//
// =============

void PlayerAI::doArmyActions() {
  spawnArmy();
  checkForEnemyTownhall();

  auto armies = gameState_->getPlayerArmies(self);

  for (Army& a : armies) {
    // if can colonize, do that
    gameState_->colonize(a);
    // randomly move it if townhall not discovered

    int targetX = enemyTownhallPosition_.first;
    int targetY = enemyTownhallPosition_.second;

    int startX = a.getLocationX();
    int startY = a.getLocationY();

    // BFS to find the shortest path to the enemy townhall
    std::queue<std::pair<int, int>> q;
    std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;
    q.push({startX, startY});
    cameFrom[{startX, startY}] = {-1, -1};

    bool found = false;

    while (!q.empty() && !found) {
      auto [x, y] = q.front();
      q.pop();

      for (const auto& neighbor :
           gameState_->getNeighbourGrassTilesNoDiagonal(x, y, 1)) {
        int nx = neighbor.getX();
        int ny = neighbor.getY();

        if (cameFrom.find({nx, ny}) == cameFrom.end() &&
            gameState_->areCoordinatesValid(nx, ny)) {
          cameFrom[{nx, ny}] = {x, y};
          q.push({nx, ny});
          if (nx == targetX && ny == targetY) {
            found = true;
            break;
          }
        }
      }
    }

    // trace the path to the townhall
    if (found) {
      std::pair<int, int> current = {targetX, targetY};
      while (cameFrom[current] != std::make_pair(startX, startY)) {
        current = cameFrom[current];
      }

      // std::cout<<"army is at " << startX << ", " << startY << " and should
      // move to " << current.first << ", " << current.second << std::endl;

      if (gameState_->isArmyWithinMovementRange(a, current.first,
                                                current.second)) {
        gameState_->moveArmy(a, current.first, current.second);
      }
    } else {
      // random movement if no path is found
      auto possibleMoves = gameState_->getArmyMovementRange(a);
      if (!possibleMoves.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, possibleMoves.size() - 1);
        int randomIndex = dis(gen);

        gameState_->moveArmy(a, possibleMoves[randomIndex].first,
                             possibleMoves[randomIndex].second);
      }
    }
  }
}

void PlayerAI::spawnArmy() { gameState_->placeSoldiers(self, INFANTRY); }

void PlayerAI::checkForEnemyTownhall() {
  if (enemyTownhallPosition_.first == -1) {
    auto townhalls = gameState_->getVisibleTownhalls(self);
    // remove own townhall
    // std::cout << "Number of townhalls in sight:" << townhalls.size() <<
    // std::endl;
    townhalls.erase(std::remove_if(townhalls.begin(), townhalls.end(),
                                   [this](const TownHall t) {
                                     return t.getOwner() == self;
                                   }),
                    townhalls.end());
    if (townhalls.size() == 1) {
      std::cout << "Townhall visible to AI!" << std::endl;
      // townhall found
      auto th = townhalls[0];
      enemyTownhallPosition_ = th.getXY();
    }
  }
}

// =============
//
// BUILD ACTIONS
//
// =============

void PlayerAI::doBuildActions() {
  auto bp = BuildingBlueprint();
  bp.setType(toPlace);

  if (canAffordBuilding(toPlace)) {
    auto claimedTiles = gameState_->getClaimedTiles(self);
    bool placed = false;

    for (auto tile : claimedTiles) {
      if (!placed && !gameState_->tileHasBuilding(tile.getX(), tile.getY()) &&
          !gameState_->tileHasTownhall(tile.getX(), tile.getY())) {
        gameState_->addBuilding(Building(bp.getType(), bp.getResourceCost(),
                                         bp.getResourceGain(), self,
                                         tile.getX(), tile.getY()));
        Resources cost = bp.getResourceCost();
        Resources negatedCost(-cost.food, -cost.wood, -cost.gold, -cost.stone);
        modifyResources(negatedCost);

        // if (toPlace == WOOD_CUTTER) {
        //     std::cout << "AI placed a Wood Cutter on " << tile.getX() << ", "
        //     << tile.getY() << std::endl;
        // } else if (toPlace == FARM) {
        //     std::cout << "AI placed a Farm on " << tile.getX() << ", " <<
        //     tile.getY()  << std::endl;
        // } else if (toPlace == MINE) {
        //     std::cout << "AI placed a Mine on " << tile.getX() << ", " <<
        //     tile.getY()  << std::endl;
        // } else if (toPlace == MARKET) {
        //     std::cout << "AI placed a Market on " << tile.getX() << ", " <<
        //     tile.getY()  << std::endl;
        // }

        setNextBuilding();
        placed = true;
      }
    }
  }
}

void PlayerAI::setNextBuilding() {
  switch (toPlace) {
    case WOOD_CUTTER:
      toPlace = FARM;
      break;

    case FARM:
      toPlace = MINE;
      break;
    case MINE:
      toPlace = MARKET;
      break;

    case MARKET:
      toPlace = WOOD_CUTTER;
      break;

    default:
      break;
  }
}

bool PlayerAI::canAffordBuilding(BuildingType building) {
  Resources cost;
  switch (building) {
    case WOOD_CUTTER:
      // 0 food, 30 wood, 0 gold, 0 stone
      cost = Resources(0, 30, 0, 0);
      break;

    case FARM:
      // 0 food, 30 wood, 0 gold, 0 stone
      cost = Resources(0, 30, 0, 0);
      break;

    case MINE:
      // 0 food, 30 wood, 10 gold, 0 stone
      cost = Resources(0, 30, 10, 0);
      break;

    case MARKET:
      // 0 food, 50 wood, 0 gold, 20 stone
      cost = Resources(0, 50, 0, 20);
      break;

    default:
      return false;
      break;
  }

  const Resources& currentResources = getResources();
  return currentResources.food >= cost.food &&
         currentResources.wood >= cost.wood &&
         currentResources.gold >= cost.gold &&
         currentResources.stone >= cost.stone;
}