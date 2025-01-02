#ifndef SRC_GAMESTATE_HPP_
#define SRC_GAMESTATE_HPP_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "army.hpp"
#include "building.hpp"
#include "player.hpp"
#include "tile.hpp"
#include "townhall.hpp"

class GameState {
 public:
  /**
   * @brief Construct a new Game State object
   *
   * @param window
   * @param turn
   */
  GameState(std::shared_ptr<sf::RenderWindow> window, int turn = 1);

  /**
   * @brief Tick function to trigger AI actions
   *
   */
  void tick();

  /**
   * @brief Adds a player to Game State
   *
   * @param player
   */
  void addPlayer(std::shared_ptr<Player> player);

  /**
   * @brief Get the Active Player ID
   *
   * @return int
   */
  int getActivePlayerID();

  /**
   * @brief Places an army at position x, y
   *
   * @param type
   * @param x
   * @param y
   * @param owner
   * @param unitCount
   */
  void addArmy(ArmyType type, int x, int y, std::shared_ptr<Player> owner,
               int unitCount = 0);

  /**
   * @brief Set the Active Player ID
   *
   * @param playerID
   */
  void setActivePlayerID(int playerID);

  /**
   * @brief Adds a building to GameState
   *
   * @param building
   */
  void addBuilding(const Building& building);

  /**
   * @brief Adds a townhall to GameState
   *
   * @param townhall
   */
  void addTownhall(const TownHall& townhall);

  /**
   * @brief Loads a map from a vector of tiles
   *
   * @param tiles
   * @param num_cols
   */
  void loadMap(const std::vector<Tile>& tiles, int num_cols);

  /**
   * @brief Loads a map from an encoded string
   *
   * @param str
   * @param num_cols
   */
  void loadMapFromString(const std::string str, int num_cols);

  /**
   * @brief Get a Tile at position (X, Y)
   *
   * @param xPos
   * @param yPos
   * @return Tile&
   */
  Tile& getTile(int xPos, int yPos);

  /**
   * @brief Get all claimed tiles by a player
   *
   * @param player Pointer to the player
   * @return Vector of claimed Tile objects
   */
  std::vector<Tile> getClaimedTiles(const std::shared_ptr<Player> player);

  /**
   * @brief Finds a TownHall at location (X, Y) if it exists
   *
   * @param xPos
   * @param yPos
   * @return an iterator
   */
  std::vector<TownHall>::iterator findTownhallByLocation(int xPos, int yPos);

  /**
   * @brief Gets a TownHall at location (X, Y) if it exists
   *
   * @param xPos
   * @param yPos
   * @return Townhall Object
   */
  TownHall& getTownhallByLocation(int xPos, int yPos);

  /**
   * @brief Finds the owner of TownHall at location (X, Y)
   *
   * @param xPos
   * @param yPos
   * @return pointer to the player
   */
  std::shared_ptr<Player> getTownhallOwner(int x, int y);

  /**
   * @brief Returns true if (X, Y) tile has a townhall on it and if the owner of
   * the townhall is the current activeplayer
   *
   * @param xPos
   * @param yPos
   * @return boolean
   */
  bool tileHasTownhall(int xPos, int yPos);

  /**
   * @brief Helper for getting all neighbours of a Tile at position (X, Y) in a
   * certain radius
   *
   * @param xPos
   * @param yPos
   * @param radius
   * @return Vector of std::vector<Tile>&
   */
  std::vector<Tile> getNeighbourTiles(int xPos, int yPos, int radius);

  /**
   * @brief Helper for getting all neighbours of a Tile at position (X, Y) in a
   * certain radius without the diagonal (useful for army)
   *
   * @param xPos
   * @param yPos
   * @param radius
   * @return std::vector<Tile>
   */
  std::vector<Tile> getNeighbourGrassTilesNoDiagonal(int xPos, int yPos,
                                                     int radius);

  /**
   * @brief Calculates soldier placement for a townhall at position (X, Y)
   *
   * @param xPos
   * @param yPos
   * @return A pair of coordinates, (-1,-1) if Soldiers cannot be placed
   * anywhere
   */
  void placeSoldiers(std::shared_ptr<Player> player, ArmyType armyType);

  /**
   * @brief Claim a Tile at position (X, Y) for a player
   *
   * @param xPos
   * @param yPos
   * @param player
   */
  void claimTile(int xPos, int yPos, const std::shared_ptr<Player>& player);

  /**
   * @brief Proceed to next turn.
   *
   * Adds and subtracts resources for players
   *
   */
  void nextTurn();

  /**
   * @brief Get the Visible XY coordinate pairs that a player can see
   *
   * @return const std::vector<Tile>&
   */
  // TODO add a reference to a player such that the map is drawn for that player
  // only (fog of war)
  std::vector<std::pair<int, int>> getVisibleXY(std::shared_ptr<Player> player);

  /**
   * @brief Get the Visible Tile objects that a player can see
   *
   * @return const std::vector<Tile>&
   */
  std::vector<Tile> getVisibleTiles(std::shared_ptr<Player> player);

  /**
   * @brief Get the Visible Building objects that a player can see
   *
   * @return const std::vector<Building>&
   */
  std::vector<Building> getVisibleBuildings(std::shared_ptr<Player> player);

  /**
   * @brief Get the Visible Townhall objects that a player can see
   *
   * @return const std::vector<TownHall>&
   */
  std::vector<TownHall> getVisibleTownhalls(std::shared_ptr<Player> player);

  /**
   * @brief Get the Visible Armie objects that a player can see
   *
   * @return const std::vector<Army>&
   */
  std::vector<Army> getVisibleArmies(std::shared_ptr<Player> player);

  /**
   * @brief Get the Army by location
   *
   * @param x
   * @param y
   * @return Army&
   */
  Army& getArmyByLocation(int x, int y);

  /**
   * @brief Helper method to find an Army by location
   *
   * @param x
   * @param y
   * @return std::vector<Army>::iterator
   */
  std::vector<Army>::iterator findArmyByLocation(int x, int y);

  /**
   * @brief Checks if the tile has an Army on it
   *
   * @param x
   * @param y
   * @return true
   * @return false
   */
  bool tileHasArmy(int x, int y);

  /**
   * @brief Return the Army movement range
   *
   * @param army
   * @return std::vector<std::pair<int, int>>
   */
  std::vector<std::pair<int, int>> getArmyMovementRange(Army& army);

  /**
   * @brief Checks if the location x, y is within the Army movement range
   *
   * @param army
   * @param x
   * @param y
   * @return true
   * @return false
   */
  bool isArmyWithinMovementRange(Army& army, int x, int y);

  /**
   * @brief Checks if the coordinates at x, y are valid
   *
   * @param x
   * @param y
   * @return true
   * @return false
   */
  bool areCoordinatesValid(int x, int y) const;

  /**
   * @brief Moves an army to position (R, C)
   *
   * @param army
   * @param x
   * @param y
   */
  void moveArmy(Army& army, int x, int y);

  /**
   * @brief Function to colonize a tile that the army is standing on
   *
   * @param army
   */
  void colonize(Army& army);

  /**
   * @brief Get player armies
   *
   * @param player
   * @return std::vector<std::reference_wrapper<Army>>
   */
  std::vector<std::reference_wrapper<Army>> getPlayerArmies(
      std::shared_ptr<Player> player);

  /**
   * @brief Finds building at location (xPos, yPos) if it exists
   *
   * @param xPos
   * @param yPos
   * @return std::vector<Building>::iterator
   */
  std::vector<Building>::iterator findBuildingByLocation(int xPos, int yPos);

  /**
   * @brief Checks tile at position (xPos, yPos) has a building on it
   *
   * @param xPos
   * @param yPos
   * @return true
   * @return false
   */
  bool tileHasBuilding(int xPos, int yPos);

  /**
   * @brief Checks if a building can be placed on the tile.
   *
   * @param x
   * @param y
   * @param playerID
   * @return true
   * @return false
   */
  bool canPlaceBuilding(int x, int y, int playerID);

  void handleGameover(std::shared_ptr<Player> player);

  std::shared_ptr<Player> getWinner() const;

  void destroyArmiesByPlayer(std::shared_ptr<Player> player);

  void destroyBuildingsByPlayer(std::shared_ptr<Player> player);

  void incrementArmyUnit(Army& army);

 private:
  std::shared_ptr<sf::RenderWindow> window_;

  int activePlayerID_;
  int turn_;
  std::vector<std::shared_ptr<Player>> players_;

  int num_rows_;
  std::vector<Tile> map_;

  std::vector<TownHall> townhalls_;
  std::vector<Building> buildings_;
  std::vector<Army> armies_;
};

#endif  // SRC_GAMESTATE_HPP_
