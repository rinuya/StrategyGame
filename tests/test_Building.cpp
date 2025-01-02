#include <cassert>
#include <iostream>
#include <memory>
#include "building.hpp"
#include "player.hpp"

void testBuildingConstructor() {
    auto owner = std::make_shared<Player>(sf::Color::Red, 1);

    Building building(WOOD_CUTTER, {}, {}, owner, 10, 15);

    assert(building.getType() == WOOD_CUTTER);
    assert(building.getOwner() == owner);
    assert(building.getX() == 10);
    assert(building.getY() == 15);
    assert(building.getId() >= 0);

    std::cout << "testBuildingConstructor passed.\n";
}

void testBuildingSetOwner() {
    auto owner1 = std::make_shared<Player>(sf::Color::Blue, 1);
    auto owner2 = std::make_shared<Player>(sf::Color::Green, 2);

    Building building(WOOD_CUTTER, {}, {}, owner1, 10, 15);
    auto previousOwner = building.setOwner(owner2);

    assert(previousOwner == owner1);
    assert(building.getOwner() == owner2);

    std::cout << "testBuildingSetOwner passed.\n";
}

void testBuildingNextTurn() {
    auto owner = std::make_shared<Player>(sf::Color::Red, 1);

    Building building(WOOD_CUTTER, {}, {}, owner, 10, 15);
    auto nextTurnResources = building.nextTurn();

    // Verify the resource values for WOOD_CUTTER
    assert(nextTurnResources.food == 0);
    assert(nextTurnResources.wood == 20);
    assert(nextTurnResources.gold == 0);
    assert(nextTurnResources.stone == 0);

    std::cout << "testBuildingNextTurn passed.\n";
}

int main() {
    testBuildingConstructor();
    testBuildingSetOwner();
    testBuildingNextTurn();

    std::cout << "All tests passed.\n";
    return 0;
}