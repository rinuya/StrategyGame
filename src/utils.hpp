#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>
#include <string>

// A file with common utility functions used across all classes.

enum ORIENTATION { START, CENTER, END };

/**
 * @brief Create a Rect object
 *
 * @param x
 * @param y
 * @param width
 * @param height
 * @param color
 * @return sf::RectangleShape
 */
sf::RectangleShape createRect(float x, float y, float width, float height,
                              sf::Color fillColor);

/**
 * @brief Create a Rect object using integers
 *
 * @param x
 * @param y
 * @param width
 * @param height
 * @param color
 * @return sf::RectangleShape
 */
sf::RectangleShape createRect(int x, int y, int width, int height,
                              sf::Color fillColor);

/**
 * @brief Create a Rect object with an outline/border
 *
 * @param width
 * @param height
 * @param x
 * @param y
 * @param fillColor
 * @param borderSize
 * @param borderColor
 * @return sf::RectangleShape
 */
sf::RectangleShape createRect(float width, float height, float x, float y,
                              sf::Color fillColor, float outlineSize,
                              sf::Color outlineColor);

/**
 * @brief Create a Rect object with an outline/border using integers
 *
 * @param width
 * @param height
 * @param x
 * @param y
 * @param fillColor
 * @param outlineSize
 * @param outlineColor
 * @return sf::RectangleShape
 */
sf::RectangleShape createRect(int width, int height, int x, int y,
                              sf::Color fillColor, int outlineSize,
                              sf::Color outlineColor);

/**
 * @brief Create the Text inside a Rect object
 *
 * Allows you to position the text freely according to the Enum ORIENTATION.
 * START, CENTER, END are valid options for x and y orientation.
 *
 * @param rect
 * @param font
 * @param text
 * @param charSize
 * @param fillColor
 * @param orientationX
 * @param orientationY
 * @return sf::Text
 */
sf::Text createTextForRect(sf::RectangleShape rect, sf::Font& font,
                           std::string text, int charSize = 18,
                           sf::Color fillColor = sf::Color::White,
                           ORIENTATION orientationX = CENTER,
                           ORIENTATION orientationY = CENTER);

/**
 * @brief Create a vector of text inside a Rect object
 *
 * Allows you to add a vector of strings as text, with each string placed on a
 * new line.
 *
 * @param rect
 * @param font
 * @param text
 * @param charSize
 * @param fillColor
 * @param offset
 * @param lineSpacing
 * @return std::vector<sf::Text>
 */
std::vector<sf::Text> createTextsForRect(sf::RectangleShape rect,
                                         sf::Font& font,
                                         std::vector<std::string> texts,
                                         int charSize = 18,
                                         sf::Color fillColor = sf::Color::White,
                                         int offset = 0, int lineSpacing = 0);

#endif  // UTILS_HPP
