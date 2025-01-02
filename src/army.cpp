#include "army.hpp"

#include <iostream>
#include <sstream>

std::string getArmyTypeName(ArmyType armyType) {
  switch (armyType) {
    case ArmyType::INFANTRY:
      return "Infantry";
    case ArmyType::CAVALRY:
      return "Cavalry";
    case ArmyType::ARTILLERY:
      return "Artillery";
    case ArmyType::MARINE:
      return "Marine";
    default:
      return "Army";
  }
}

Resources getArmyDeploymentCost(ArmyType armyType) {
  switch (armyType) {
    case ArmyType::INFANTRY:
      return Resources(10, 0, 0, 0);
    case ArmyType::CAVALRY:
      return Resources(20, 0, 0, 0);
    case ArmyType::ARTILLERY:
      return Resources(10, 0, 10, 10);
    case ArmyType::MARINE:
      return Resources(10, 10, 0, 0);
    default:
      return Resources(0, 0, 0, 0);
  }
}

Army::Army(ArmyType type, std::pair<int, int> location,
           std::shared_ptr<Player> owner, int unitCount)
    : Selectable(getArmyTypeName(type)),
      owner_(owner),
      location_(location),
      unitCount_(unitCount),
      hasCompletedTurn_(false),
      type_(type) {
  switch (type) {
    case ArmyType::INFANTRY:
      attack_ = 2;
      defense_ = 2;
      break;
    case ArmyType::CAVALRY:
      attack_ = 3;
      defense_ = 3;
      break;
    case ArmyType::ARTILLERY:
      attack_ = 100;
      defense_ = 100;
      break;
    case ArmyType::MARINE:
      attack_ = 2;
      defense_ = 2;
      break;
  }
}

std::shared_ptr<Player> Army::getOwner() const { return owner_; };

std::shared_ptr<Player> Army::setOwner(std::shared_ptr<Player> player) {
  std::shared_ptr<Player> currentOwner = owner_;
  owner_ = player;
  return currentOwner;
};

std::pair<int, int> Army::getLocation() const { return location_; };

int Army::getLocationX() const { return location_.first; };

int Army::getLocationY() const { return location_.second; };

std::pair<int, int> Army::setLocation(std::pair<int, int> location) {
  std::pair<int, int> currentLocation = location_;
  location_ = location;
  return currentLocation;
};

int Army::getUnitCount() const { return unitCount_; };

void Army::setUnitCount(int unitCount) { unitCount_ = unitCount; };

const std::string Army::toString() const {
  std::stringstream ss;
  ss << getName() << std::endl;
  ss << "Owner: " << owner_->getID() << std::endl;
  ss << "Location: (" << location_.first << ", " << location_.second << ")"
     << std::endl;
  ss << "Unit Count: " << unitCount_ << std::endl;
  return ss.str();
}

bool Army::getHasCompletedTurn() const { return hasCompletedTurn_; };

void Army::setHasCompletedTurn(bool hasCompletedTurn) {
  hasCompletedTurn_ = hasCompletedTurn;
}

ArmyType Army::getType() const { return type_; };
int Army::getAttack() const { return attack_; };
int Army::getDefense() const { return defense_; };
bool Army::canAdvance(TileType type) {
  return type == GRASS || (type_ == MARINE && type == WATER);
};
void Army::incrementUnitCount() { unitCount_++; }