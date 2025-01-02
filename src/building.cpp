#include "building.hpp"

BuildingBlueprint::BuildingBlueprint() : Selectable("Blueprint") {
  std::cout << "BuildingBlueprint constructor called, setting label to 'Blueprint'" << std::endl;
  setLabel("Blueprint");
}

BuildingType BuildingBlueprint::getType() const { return type_; }

void BuildingBlueprint::setType(BuildingType type) {
  type_ = type;
  switch (type) {
    case WOOD_CUTTER:
      setLabel("Wood Cutter");
      // 0 food, 30 wood, 0 gold, 0 stone
      resourceCost_ = Resources(0, 50, 0, 0);
      // Gain 15 wood per turn
      resourceGain_ = Resources(0, 15, 0, 0);
      break;
    case FARM:
      setLabel("Farm");
      // 0 food, 30 wood, 0 gold, 0 stone
      resourceCost_ = Resources(0, 40, 0, 0);
      // Gain 20 food per turn
      resourceGain_ = Resources(20, 0, 0, 0);
      break;
    case MINE:
      setLabel("Mine");
      // 0 food, 30 wood, 10 gold, 0 stone
      resourceCost_ = Resources(0, 30, 10, 0);
      // Gain 10 stone per turn
      resourceGain_ = Resources(0, 0, 0, 10);
      break;
    case MARKET:
      setLabel("Market");
      // 0 food, 50 wood, 0 gold, 20 stone
      resourceCost_ = Resources(0, 50, 0, 20);
      // Gain 5 gold per turn
      resourceGain_ = Resources(0, 0, 5, 0);
      break;
    default:
      break;
  }
}

Resources BuildingBlueprint::getResourceCost() const { return resourceCost_; };

Resources BuildingBlueprint::getResourceGain() const { return resourceGain_; };

std::atomic<int> Building::idCounter_{0};

Building::Building(BuildingType type, Resources resourceCost,
                   Resources resourceGain, std::shared_ptr<Player> owner, int x,
                   int y)
    : Selectable("Building"),
      type_(type),
      resourceCost_(resourceCost),
      resourceGain_(resourceGain),
      owner_(owner),
      x_(x),
      y_(y) {
  id_ = idCounter_++;
  // resource cost depends on building type.
}

int Building::getId() const { return id_; }

int Building::getX() const { return x_; }

int Building::getY() const { return y_; }

std::pair<int, int> Building::getXY() const {
  return std::make_pair(getX(), getY());
}

BuildingType Building::getType() const { return type_; }

std::shared_ptr<Player> Building::setOwner(std::shared_ptr<Player> player) {
  auto previousOwner = owner_;
  owner_ = player;
  return previousOwner;
}

std::shared_ptr<Player> Building::getOwner() const { return owner_; }

Resources Building::getResourceCost() const { return resourceCost_; }

Resources Building::nextTurn() const {
  switch (type_) {
    case WOOD_CUTTER:
      return Resources(0, 20, 0, 0);
    case FARM:
      return Resources(20, 0, 0, 0);
    case MINE:
      return Resources(0, 0, 0, 20);
    case MARKET:
      return Resources(0, 0, 20, 0);
    default:
      return Resources(0, 0, 0, 0);
  }
}

std::vector<std::string> Building::getInfo() const {
  std::vector<std::string> lines;

  // owner information
  std::string ownerName = "Owner: None";

  if (owner_ != nullptr) {
    ownerName = "Owner: Player ";
    ownerName += std::to_string(owner_->getID());
  }

  lines.push_back(ownerName);

  // building type information
  lines.push_back("Building Type: " + getLabel());

  //  resource information
  std::ostringstream resourceInfo;
  Resources passiveResources = nextTurn();

  if (passiveResources.food > 0) {
    resourceInfo << "Provides: " << passiveResources.food << " Food";
    lines.push_back(resourceInfo.str());
    resourceInfo.str("");
    resourceInfo.clear();
  }

  if (passiveResources.wood > 0) {
    resourceInfo << "Provides: " << passiveResources.wood << " Wood";
    lines.push_back(resourceInfo.str());
    resourceInfo.str("");
    resourceInfo.clear();
  }

  if (passiveResources.gold > 0) {
    resourceInfo << "Provides: " << passiveResources.gold << " Gold";
    lines.push_back(resourceInfo.str());
    resourceInfo.str("");
    resourceInfo.clear();
  }

  if (passiveResources.stone > 0) {
    resourceInfo << "Provides: " << passiveResources.stone << " Stone";
    lines.push_back(resourceInfo.str());
  }

  return lines;
}
