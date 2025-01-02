#ifndef SRC_ARMY_HPP
#define SRC_ARMY_HPP

#include <map>
#include <memory>

#include "player.hpp"
#include "tile.hpp"

enum ArmyType { INFANTRY, CAVALRY, ARTILLERY, MARINE };

std::string getArmyTypeName(ArmyType armyType);
Resources getArmyDeploymentCost(ArmyType armyType);

class Army : public Selectable {
 public:
  Army(ArmyType type, std::pair<int, int> location,
       std::shared_ptr<Player> owner = nullptr, int unitCount = 0);

  std::shared_ptr<Player> getOwner() const;
  std::shared_ptr<Player> setOwner(std::shared_ptr<Player> player);
  std::pair<int, int> getLocation() const;
  int getLocationX() const;
  int getLocationY() const;
  std::pair<int, int> setLocation(std::pair<int, int> location);
  int getUnitCount() const;
  void setUnitCount(int unitCount);
  const std::string toString() const;
  bool getHasCompletedTurn() const;
  void setHasCompletedTurn(bool hasCompletedTurn);
  ArmyType getType() const;
  int getAttack() const;
  int getDefense() const;
  bool canAdvance(TileType type);
  void incrementUnitCount();

 private:
  ArmyType type_;
  std::shared_ptr<Player> owner_;
  std::pair<int, int> location_;
  int unitCount_;
  bool hasCompletedTurn_;
  int attack_;
  int defense_;
};

#endif