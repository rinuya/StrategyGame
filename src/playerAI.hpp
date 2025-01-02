#ifndef SRC_PLAYERAI_HPP_
#define SRC_PLAYERAI_HPP_

#include <chrono>
#include <queue>
#include <random>
#include <thread>
#include <vector>

#include "gamestate.hpp"
#include "player.hpp"

class PlayerAI : public Player {
 public:
  /**
   * @brief Create a new PlayerAI
   *
   * @param color
   * @param turnOrder
   * @param gameState
   * @param difficulty
   * @param resources
   */
  PlayerAI(sf::Color color, int turnOrder, std::shared_ptr<GameState> gameState,
           Resources resources = Resources(), int difficulty = 0)
      : Player(color, turnOrder, resources),
        gameState_(std::move(gameState)),
        difficulty_(difficulty) {}

  /**
   * @brief Takes all PlayerAI actions for the turn.
   *
   */
  void doTurn() override { takeTurnActions(); };

  /**
   * @brief A function to add the AI's own pointer to itself
   *
   * @param refToSelf
   */
  void addSharedPtr(std::shared_ptr<PlayerAI> refToSelf);

 private:
  /**
   * @brief Takes all turn actions in order
   *
   */
  void takeTurnActions();

  /**
   * @brief Takes all army actions
   *
   */
  void doArmyActions();

  /**
   * @brief Creates 1 army unit at townhall
   *
   */
  void spawnArmy();

  /**
   * @brief Stores position of enemy townhall if it is in sight
   *
   */
  void checkForEnemyTownhall();

  /**
   * @brief Takes all build actions
   *
   */
  void doBuildActions();

  /**
   * @brief Selects the next building that the AI will place
   *
   * @return BuildingType
   */
  void setNextBuilding();

  /**
   * @brief Returns whether the AI can afford a building.
   *
   * @param building
   * @return true
   * @return false
   */
  bool canAffordBuilding(BuildingType building);

  std::shared_ptr<GameState> gameState_;
  std::shared_ptr<PlayerAI> self;
  int difficulty_;
  int currentActionStep_ = 0;
  const int maxSteps_ = 1;
  // init to market such that woodcutter comes next
  BuildingType toPlace = WOOD_CUTTER;
  // as soon as enemy townhall is spotted, the coords will save it to here. -1
  // means not found
  std::pair<int, int> enemyTownhallPosition_ = std::make_pair(-1, -1);
  ;
};

#endif  // SRC_PLAYERAI_HPP_
