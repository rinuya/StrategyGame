#include <cassert>
#include <iostream>

#include "player.hpp"

void testPlayerConstructor() {
    Resources initialResources(200, 300, 50, 20);
    Player player(sf::Color::Red, 1, initialResources);

    assert(player.getResources().food == 200);
    assert(player.getResources().wood == 300);
    assert(player.getResources().gold == 50);
    assert(player.getResources().stone == 20);
    assert(player.getColor() == sf::Color::Red);
    assert(player.getID() >= 0);
    std::cout << "testPlayerConstructor passed.\n";
}

void testModifyResources() {
    Player player(sf::Color::Blue, 2);

    player.modifyResources(Resources(50, 50, 10, 5));
    assert(player.getResources().food == 150);
    assert(player.getResources().wood == 150);
    assert(player.getResources().gold == 20);
    assert(player.getResources().stone == 5);

    player.modifyResources(Resources(-30, -50, -10, 0));
    assert(player.getResources().food == 120);
    assert(player.getResources().wood == 100);
    assert(player.getResources().gold == 10);
    assert(player.getResources().stone == 5);

    std::cout << "testModifyResources passed.\n";
}

void testPlayerColor() {
    Player player(sf::Color::Green, 3);
    assert(player.getColor() == sf::Color::Green);
    std::cout << "testPlayerColor passed.\n";
}

void testPlayerID() {
    Player player1(sf::Color::Yellow, 1);
    Player player2(sf::Color::Red, 2);

    assert(player1.getID() != player2.getID());
    assert(player1.getID() < player2.getID());

    std::cout << "testPlayerID passed.\n";
}

int main() {
    testPlayerConstructor();
    testModifyResources();
    testPlayerColor();
    testPlayerID();

    std::cout << "All tests passed.\n";
    return 0;
}
