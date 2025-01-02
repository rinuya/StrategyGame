#ifndef SRC_TILE_HPP_
#define SRC_TILE_HPP_

#include <memory>
#include <utility>

#include "player.hpp"
#include "selectable.hpp"

enum TileType { GRASS, WATER, ROCK, PLACEHOLDER };

class Tile : public Selectable {
 public:
  Tile(int x, int y, TileType type = GRASS,
       std::shared_ptr<Player> owner = nullptr);

  /**
   * get the X coordinate of this tile.
   * @return This tile's X coordinate.
   **/
  int getX() const;

  /**
   * get the Y coordinate of this tile.
   * @return This tile's Y coordinate.
   **/
  int getY() const;

  /**
   * get the a coordinate pair for this tile.
   * @return This tile's coordinate pair <X, Y>.
   **/
  std::pair<int, int> getXY() const;

  /**
   * get the type of this tile.
   * @return This tile's TileType.
   **/
  TileType getType() const;

  /**
   * set the type of the tyle to a new type
   * @returnnothing
   **/
  void setType(TileType type);

  /**
   * set the owner of this tile.
   * @param player pointer to the new tile owner.
   * @return The prior owner.
   **/
  std::shared_ptr<Player> setOwner(std::shared_ptr<Player> player);

  /**
   * get the owner of this tile.
   * @return This tile's owner.
   **/
  std::shared_ptr<Player> getOwner() const;

  /**
   * set the occupation status of this tile.
   * @param occupy whether or not this tile should be occupied.
   **/
  void setOccupied(bool occupy);

  /**
   * check if this tile is occupied.
   * @return Occupation status as a bool.
   **/
  bool isOccupied() const;

  std::vector<std::string> getInfo() const override;

 private:
  int x_;
  int y_;
  TileType type_;
  std::shared_ptr<Player> owner_;
  bool occupied_;
};

#endif  // SRC_TILE_HPP_
