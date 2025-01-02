#include "townhall.hpp"

TownHall::TownHall(int id, int soldierCost, std::shared_ptr<Player> owner,
                   int x, int y)
    : Selectable("TownHall"),
      id_(id),
      soldierCost_(soldierCost),
      owner_(owner),
      x_(x),
      y_(y) {}

int TownHall::getId() const { return id_; }

std::shared_ptr<Player> TownHall::getOwner() const { return owner_; };

std::string TownHall::getName() const { return getLabel(); }

std::string TownHall::getLabel() const { return label; }

std::vector<std::string> TownHall::getInfo() const {
  std::vector<std::string> result;
  result.push_back(getName());
  result.push_back("Soldier cost: " + std::to_string(getSoldierCost()));
  result.push_back(" Food");

  return result;
}

bool TownHall::canRecruitSoldier(Resources resources, ArmyType armyType) const {
  Resources cost = getArmyDeploymentCost(armyType);

  return resources.food >= cost.food && resources.wood >= cost.wood &&
         resources.gold >= cost.gold && resources.stone >= cost.stone;
}

std::pair<int, int> TownHall::getPosition() const { return {x_, y_}; }

// Pretty Straightforward, just helpers
int TownHall::getX() const { return x_; }

int TownHall::getY() const { return y_; }

std::pair<int, int> TownHall::getXY() const {
  return std::make_pair(getX(), getY());
}

int TownHall::getSoldierCost() const { return soldierCost_; }
