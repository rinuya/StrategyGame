#include "player.hpp"

std::atomic<int> Player::idCounter_{1};

Player::Player(sf::Color color, int turnOrder, Resources resources)
    : color_(color),
      turnOrder_(turnOrder),
      resources_(resources),
      isAlive_(true) {
  // Automatically generate ID for each player
  id_ = idCounter_++;
}

const Resources& Player::getResources() const { return resources_; }

void Player::modifyResources(const Resources& delta) {
  // resources cannot drop under 0
  resources_.food = std::max(0, resources_.food + delta.food);
  resources_.wood = std::max(0, resources_.wood + delta.wood);
  resources_.gold = std::max(0, resources_.gold + delta.gold);
  resources_.stone = std::max(0, resources_.stone + delta.stone);
}

int Player::getID() { return id_; }

sf::Color Player::getColor() { return color_; };

bool Player::getIsAlive() const { return isAlive_; }

void Player::setIsAlive(bool isAlive) { isAlive_ = isAlive; }
