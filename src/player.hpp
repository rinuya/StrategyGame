#ifndef SRC_PLAYER_HPP_
#define SRC_PLAYER_HPP_

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <atomic>

struct Resources {
  int food = 100;
  int wood = 100;
  int gold = 10;
  int stone = 0;

  Resources() = default;

  // Custom constructor to init resources
  Resources(int food, int wood, int gold, int stone)
      : food(food), wood(wood), gold(gold), stone(stone) {}
};

class Player {
 public:
  /**
   * @brief Construct a new Player object
   *
   * @param color
   * @param turnOrder
   * @param resources
   */
  Player(sf::Color color, int turnOrder, Resources resources = Resources());

  /**
   * @brief Destroy the Player object
   *
   */
  virtual ~Player() = default;

  /**
   * @brief Get the Resource struct
   *
   * @return const Resources&
   */
  const Resources& getResources() const;

  /**
   * @brief Change player resources
   *
   * @param delta can be negative or positive values
   */
  void modifyResources(const Resources& delta);

  /**
   * @brief Get Player ID
   *
   * @return int
   */
  int getID();

  /**
   * @brief Get the Player Color object
   *
   * @return sf::Color
   */
  sf::Color getColor();

  /**
   * @brief Function for turn behavior.
   *
   */
  virtual void doTurn() {};

  bool getIsAlive() const;

  void setIsAlive(bool isAlive);

 private:
  int id_;
  Resources resources_;
  int turnOrder_;
  sf::Color color_;
  static std::atomic<int> idCounter_;
  bool isAlive_;
};

#endif  // SRC_PLAYER_HPP_