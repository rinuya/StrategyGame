#include "tile.hpp"
#include <cassert>
#include <iostream>
#include <memory>

void testTileXY() {
    Tile tile(10, 100);
    assert(tile.getXY() == std::make_pair(10, 100));
    std::cout << "testTileXY passed.\n";
}

void testTileType() {
    Tile tile(0, 0, WATER);
    assert(tile.getType() == WATER);
    tile.setType(ROCK);
    assert(tile.getType() == ROCK);
    tile.setType(GRASS);
    assert(tile.getType() == GRASS);
    tile.setType(PLACEHOLDER);
    assert(tile.getType() == PLACEHOLDER);
    std::cout << "testTileType passed.\n";
}

void testTileOwner() {
    Tile tile(0, 0);
    assert(tile.getOwner() == nullptr);
    std::shared_ptr<Player> playerPtr = std::make_shared<Player>(sf::Color(0,128,128), 1);
    tile.setOwner(playerPtr);
    assert(tile.getOwner() == playerPtr);
    std::cout << "testTileOwner passed.\n";
}

void testTileOccupied() {
    Tile tile(0, 0);
    tile.setOccupied(false);
    assert(tile.isOccupied() == false);
    tile.setOccupied(true);
    assert(tile.isOccupied() == true);
    std::cout << "testTileOccupied passed.\n";
}

int main() {
    testTileXY();
    testTileType();
    testTileOwner();
    testTileOccupied();

    std::cout << "All tests passed.\n";
    return 0;
}