#include <cassert>
#include <iostream>
#include <memory>

#include "townhall.hpp"

void testTownHallConstructor() {
    auto player = std::make_shared<Player>(sf::Color::Blue, 1);
    TownHall townHall(1, 100, player, 5, 10);

    assert(townHall.getId() == 1);
    assert(townHall.getSoldierCost() == 100);
    assert(townHall.getOwner() == player);
    assert(townHall.getX() == 5);
    assert(townHall.getY() == 10);
    assert(townHall.getXY() == std::make_pair(5, 10));

    std::cout << "testTownHallConstructor passed.\n";
}

void testTownHallGetInfo() {
    auto player = std::make_shared<Player>(sf::Color::Red, 2);
    TownHall townHall(2, 150, player, 7, 14);

    auto info = townHall.getInfo();
    assert(info.size() == 3);
    assert(info[0] == townHall.getName());
    assert(info[1] == "Soldier cost: 150");
    assert(info[2] == " Food");

    std::cout << "testTownHallGetInfo passed.\n";
}

void testCanRecruitSoldier() {
    auto player = std::make_shared<Player>(sf::Color::Green, 3);
    TownHall townHall(3, 120, player, 10, 20);

    Resources sufficientResources{15, 0, 0, 0};
    Resources insufficientResources{5, 0, 0, 0};

    ArmyType testArmyType = INFANTRY;
    assert(townHall.canRecruitSoldier(sufficientResources, testArmyType) == true);
    assert(townHall.canRecruitSoldier(insufficientResources, testArmyType) ==
          false);

    std::cout << "testCanRecruitSoldier passed.\n";
}

void testTownHallPosition() {
    auto player = std::make_shared<Player>(sf::Color::Yellow, 4);
    TownHall townHall(4, 200, player, 15, 25);

    auto position = townHall.getPosition();
    assert(position.first == 15);
    assert(position.second == 25);

    std::cout << "testTownHallPosition passed.\n";
}

int main() {
    testTownHallConstructor();
    testTownHallGetInfo();
    testCanRecruitSoldier();
    testTownHallPosition();

    std::cout << "All tests passed.\n";
    return 0;
}
