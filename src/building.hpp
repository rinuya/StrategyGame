#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <sstream>
#include <utility>

#include "player.hpp"
#include "selectable.hpp"

enum BuildingType { WOOD_CUTTER, FARM, MARKET, MINE };

class BuildingBlueprint : public Selectable {
 public:
  /**
   * @brief Construct a new Building Blueprint object
   *
   */
  BuildingBlueprint();

  /**
   * @brief Get the Type object
   *
   * @return BuildingType
   */
  BuildingType getType() const;

  void setType(BuildingType type);

  Resources getResourceCost() const;

  Resources getResourceGain() const;

 private:
  BuildingType type_;
  Resources resourceCost_;
  Resources resourceGain_;
};

class Building : public Selectable {
 public:
  /**
   * @brief Construct a new Building object
   *
   * @param id
   * @param type
   * @param resourceCost
   * @param owner
   * @param x
   * @param y
   */
  Building(BuildingType type, Resources resourceCost, Resources resourceGain,
           std::shared_ptr<Player> owner = nullptr, int x = 0, int y = 0);

  /**
   * @brief Get the Id of the building.
   *
   * @return Building Id.
   */
  int getId() const;

  /**
   * @brief Get the x-coordinate of the building.
   *
   * @return Returns the building's x-coordinate.
   */
  int getX() const;

  /**
   * @brief Get the y-coordinate of the building.
   *
   * @return Returns the building's x-coordinate.
   */
  int getY() const;

  /**
   * @brief Get the coordinates of the building as a pair.
   *
   * @return Pair for building coordinates.
   */
  std::pair<int, int> getXY() const;

  /**
   * @brief Get the type of building.
   *
   * @return Building's type.
   */
  BuildingType getType() const;

  /**
   * @brief Set the owner of the building.
   *
   * @param player
   * @return Previous owner.
   */
  std::shared_ptr<Player> setOwner(std::shared_ptr<Player> player);

  /**
   * @brief Get the owner of the building.
   *
   * @return Current owner.
   */
  std::shared_ptr<Player> getOwner() const;

  /**
   * @brief Get the resource cost for the building.
   *
   * @return The resource cost.
   */
  Resources getResourceCost() const;

  /**
   * @brief Get information about the building
   * The information includes:
   * - the owner of the building
   * - the type of the building
   * - the resources
   *
   * @return std::vector<std::string>
   */
  std::vector<std::string> getInfo() const override;

  /**
   * @brief Returns the resources provided by the building during the next turn
   *
   * The type and amount of resources depend on the building type:
   *
   * - wood cutter: 20 wood
   * - farm: 20 food
   * - mine: 20 stone
   * - market: 20 gold
   *
   * @return Resources
   */
  Resources nextTurn() const;

 private:
  int id_;
  BuildingType type_;
  Resources resourceCost_;
  Resources resourceGain_;
  std::shared_ptr<Player> owner_;
  int x_;
  int y_;
  static std::atomic<int> idCounter_;
};

#endif