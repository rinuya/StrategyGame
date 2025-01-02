// utils.cpp
#include "utils.hpp"

#include <iostream>

sf::RectangleShape createRect(float x, float y, float width, float height,
                              sf::Color fillColor) {
  sf::RectangleShape rectangle(sf::Vector2f(width, height));
  rectangle.setPosition(x, y);
  rectangle.setFillColor(fillColor);
  return rectangle;
}

sf::RectangleShape createRect(int x, int y, int width, int height,
                              sf::Color fillColor) {
  return createRect((float)width, (float)height, float(x), float(y), fillColor);
}

sf::RectangleShape createRect(float x, float y, float width, float height,
                              sf::Color fillColor, float outlineSize,
                              sf::Color outlineColor) {
  sf::RectangleShape rectangle(sf::Vector2f(width, height));
  rectangle.setPosition(x, y);
  rectangle.setFillColor(fillColor);
  rectangle.setOutlineThickness(outlineSize);
  rectangle.setOutlineColor(outlineColor);
  return rectangle;
}

sf::RectangleShape createRect(int x, int y, int width, int height,
                              sf::Color fillColor, int outlineSize,
                              sf::Color outlineColor) {
  return createRect((float)width, (float)height, float(x), float(y), fillColor,
                    (float)outlineSize, outlineColor);
}

sf::Text createTextForRect(sf::RectangleShape rect, sf::Font& font,
                           std::string text, int charSize, sf::Color fillColor,
                           ORIENTATION orientationX, ORIENTATION orientationY) {
  sf::Text t;
  t.setFont(font);
  t.setString(text);
  t.setFillColor(fillColor);
  t.setCharacterSize(charSize);

  // Get the text's bounds after setting the string
  sf::FloatRect textBounds = t.getLocalBounds();
  // Get rectangle properties
  sf::Vector2f rectPos = rect.getPosition();
  sf::Vector2f rectSize = rect.getSize();

  // Determine the x-origin and position based on orientationX
  float originX, posX;
  if (orientationX == START) {
    originX = -10;     // Align text to the left
    posX = rectPos.x;  // Position at the rectangle's left
  } else if (orientationX == CENTER) {
    originX = textBounds.width / 2;  // Center text horizontally
    posX = rectPos.x + rectSize.x / 2;
  } else {                          // orientationX == END
    originX = textBounds.width;     // Align text to the right
    posX = rectPos.x + rectSize.x;  // Position at the rectangle's right
  }

  // Determine the y-origin and position based on orientationY
  float originY, posY;
  if (orientationY == START) {
    originY = -10;     // Align text to the top
    posY = rectPos.y;  // Position at the rectangle's top
  } else if (orientationY == CENTER) {
    originY = textBounds.height / 2;  // Center text vertically
    posY = rectPos.y + rectSize.y / 2;
  } else {                          // orientationY == END
    originY = textBounds.height;    // Align text to the bottom
    posY = rectPos.y + rectSize.y;  // Position at the rectangle's bottom
  }

  t.setOrigin(originX, originY);
  t.setPosition(posX, posY);

  return t;
}

std::vector<sf::Text> createTextsForRect(sf::RectangleShape rect,
                                         sf::Font& font,
                                         std::vector<std::string> texts,
                                         int charSize, sf::Color fillColor,
                                         int offset, int lineSpacing) {
  std::vector<sf::Text> ts;
  int row = 0;
  for (const std::string text : texts) {
    sf::Text t;
    t.setFont(font);
    t.setString(text);
    t.setFillColor(fillColor);
    t.setCharacterSize(charSize);

    // Get the text's bounds after setting the string
    sf::FloatRect textBounds = t.getLocalBounds();
    // Get rectangle properties
    sf::Vector2f rectPos = rect.getPosition();
    sf::Vector2f rectSize = rect.getSize();

    // Determine the x-origin and position
    float originX, posX;
    originX = -10;     // Align text to the left
    posX = rectPos.x;  // Position at the rectangle's left

    // Determine the y-origin and position based on offset and row
    float originY, posY;
    originY = -10;  // Align text to the top
    posY = rectPos.y + offset +
           row * (charSize + lineSpacing);  // Position based on offset + row *
                                            // (character size + line spacing)

    t.setOrigin(originX, originY);
    t.setPosition(posX, posY);
    if (posY < rectPos.y + rectSize.y) {
      ts.push_back(t);
    } else {
      std::cout << "Text that was going to be placed outside a rectangle was "
                   "skipped: "
                << text << std::endl;
    }
    row++;
  }

  return ts;
}
