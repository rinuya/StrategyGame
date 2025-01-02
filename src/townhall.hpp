#ifndef TOWNHALL_HPP
#define TOWNHALL_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>

#include "army.hpp"
#include "player.hpp"
#include "selectable.hpp"

class TownHall : public Selectable {
 public:
  /**
   * @brief Construct a new TownHall object.
   *
   * @param id
   * @param soldierCost Cost to recruit a soldier.
   * @param owner
   * @param x
   * @param y
   */
  TownHall(int id, int soldierCost, std::shared_ptr<Player> owner, int x,
           int y);

  /**
   * @brief Get the ID of the Town Hall.
   *
   * @return Town Hall ID.
   */
  int getId() const;

  /**
   * @brief Check if there are enough resources to recruit a soldier.
   *
   * @return True if resources are sufficient, false otherwise.
   */
  bool canRecruitSoldier(Resources resources, ArmyType armyType) const;

  /**
   * @brief Returns owner of the Townhall.
   *
   * @return Pointer to player
   */
  std::shared_ptr<Player> getOwner() const;

  /**
   * @brief Returns information about the townhall.
   *
   * @return string vector
   */
  std::vector<std::string> getInfo() const;

  /**
   * @brief Returns the name of the townhall (It's just the label for now).
   *
   * @return string
   */
  std::string getName() const;

  /**
   * @brief Returns label of the townhall.
   *
   * @return string
   */
  std::string getLabel() const;

  /**
   * @brief Get the Town Hall's position on the grid.
   *
   * @return Pair of x and y coordinates.
   */
  std::pair<int, int> getPosition() const;

  // This is pretty straightforward I won't make a doc for it lmao
  int getX() const;
  int getY() const;

  /**
   * get the a coordinate pair for this tile.
   * @return This tile's coordinate pair <X, Y>.
   **/
  std::pair<int, int> getXY() const;

  /**
   * @brief Get the resource cost of recruiting.
   *
   * @return The resource cost.
   */
  int getSoldierCost() const;

  // TODO:Getinforstring for the info box

 private:
  int id_;
  int soldierCost_;
  std::shared_ptr<Player> owner_;
  std::string label = "Townhall";
  int x_;
  int y_;

  void initializeShape();
};

#endif