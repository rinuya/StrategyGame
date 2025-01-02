#include "tile.hpp"

Tile::Tile(int x, int y, TileType type, std::shared_ptr<Player> owner)
    : Selectable("Tile"), x_(x), y_(y), type_(type), owner_(owner) {}

int Tile::getX() const { return x_; }

int Tile::getY() const { return y_; }

std::pair<int, int> Tile::getXY() const {
  return std::make_pair(getX(), getY());
}

TileType Tile::getType() const { return type_; }

void Tile::setType(TileType type) { type_ = type; }

std::shared_ptr<Player> Tile::setOwner(std::shared_ptr<Player> player) {
  std::shared_ptr<Player> currentOwner = owner_;
  owner_ = player;
  return currentOwner;
}

std::shared_ptr<Player> Tile::getOwner() const { return owner_; }

void Tile::setOccupied(bool occupy) { occupied_ = occupy; }

bool Tile::isOccupied() const { return occupied_; }

std::vector<std::string> Tile::getInfo() const {
  std::vector<std::string> lines;
  std::string ownerName = "Owner: None";
  if (owner_ != nullptr) {
    ownerName = "Owner: Player ";
    ownerName += std::to_string(owner_->getID());
  }
  lines.push_back(ownerName);
  return lines;
}
