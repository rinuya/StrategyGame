#include <cassert>
#include <iostream>

#include "army.hpp"

void testArmyConstructor() {
    Resources initialResources(0, 0, 0, 0);
    std::shared_ptr<Player> owner =
        std::make_shared<Player>(sf::Color::Red, 1, initialResources);

    ArmyType type = INFANTRY;
    int x = 2;
    int y = 3;
    int unitCount = 4;
    Army army(type, std::make_pair(x, y), owner, unitCount);

    assert(army.getType() == type);
    assert(army.getLocationX() == x);
    assert(army.getLocationY() == y);
    assert(army.getOwner() == owner);
    assert(army.getUnitCount() == unitCount);
    std::cout << "testArmyConstructor passed." << std::endl;
}

void testUnitCount() {
    Resources initialResources(0, 0, 0, 0);
    std::shared_ptr<Player> owner =
        std::make_shared<Player>(sf::Color::Red, 1, initialResources);

    ArmyType type = INFANTRY;
    int x = 2;
    int y = 3;
    int unitCount = 4;
    Army army(type, std::make_pair(x, y), owner, unitCount);

    assert(army.getUnitCount() == unitCount);

    int newUnitCount = 6;
    army.setUnitCount(newUnitCount);

    assert(army.getUnitCount() == newUnitCount);

    std::cout << "testSetUnitCount passed." << std::endl;
}

void testCanAdvance() {
    Resources initialResources(0, 0, 0, 0);
    std::shared_ptr<Player> owner =
        std::make_shared<Player>(sf::Color::Red, 1, initialResources);

    int x = 2;
    int y = 3;
    int unitCount = 4;
    Army infantry(INFANTRY, std::make_pair(x, y), owner, unitCount);

    assert(infantry.canAdvance(GRASS));
    assert(!infantry.canAdvance(WATER));
    assert(!infantry.canAdvance(ROCK));

    Army marine(MARINE, std::make_pair(x, y), owner, unitCount);

    assert(marine.canAdvance(GRASS));
    assert(marine.canAdvance(WATER));
    assert(!marine.canAdvance(ROCK));

    std::cout << "testUnitTypes passed." << std::endl;
}

int main() {
    testArmyConstructor();
    testUnitCount();
    testCanAdvance();
    return 0;
}