#ifndef SRC_MAPEDITOR_HPP_
#define SRC_MAPEDITOR_HPP_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "../tile.hpp"
#include "../uimanager.hpp"
#include "../utils.hpp"

class MapEditor {
 public:
  /**
   * @brief Construct a new MapEditor object
   *
   * @param window
   * @param font
   * @param uiManager
   */
  MapEditor(UIManager& uiManager, sf::Font font,
            std::shared_ptr<sf::RenderWindow> window);

  /**
   * @brief Displays (draws) the mapeditor UI on the render window.
   *
   */
  void displayUI();

  /**
   * @brief Displays (draws) the map UI on the render window.
   *
   */
  void displayMap();

  /**
   * @brief Displays (draws) the menu UI on the render window. (for tile
   * changing in this case)
   *
   */
  void displayMenu();

  /**
   * @brief Displays (draws) the save button on the render window.
   *
   */
  void displaySave();

  /**
   * @brief Displays (draws) a tile on the map.
   * @param tile
   * @param xPos
   * @param yPos
   * @param isHighlighted
   */
  void drawTile(const Tile& tile, int xPos, int yPos,
                bool isHighlighted = false);

  /**
   * @brief Displays (draws) highlighted tiles on the map.
   *
   */
  void displayHighlightedtiles();

  /**
   * @brief Adds clicked tile to be highlighted and changes its type to the
   * current selected type.
   * @param xPos
   * @param yPos
   */
  void processTileSelected(int xPos, int yPos);

  /**
   * @brief Adds highlighted tile to the respective vector.
   * @param xPos
   * @param yPos
   */
  void addHighlightedTile(int xPos, int yPos);

  /**
   * @brief Checks if selected coords are valid on the tile map.
   * @param x
   * @param y
   */
  bool areCoordinatesValid(int x, int y) const;

  /**
   * @brief Saves the map in its current state to a file named "saved_map.txt"
   * into build/maps.
   * @param filename custom filenam for saving
   */
  void saveMapToFile(const std::string& filename) const;

  /**
   * @brief Get tile copy pasted from gamestate.
   * Gets the tile object from the map.
   * @param xPox
   * @param yPox
   * @return Tile object
   */
  Tile& getTile(int xPos, int yPos);

  /**
   * @brief Processes which menu element was clicked. Used for selecting which
   * tyle type to place on map.
   * @param event
   */
  void processMenuSelected(sf::Event event);

  /**
   * @brief Processes the MouseButtonPressed event in the mainmenu UI.
   *
   * @param event
   */
  void processMouseButtonPressed(sf::Event event);

  /**
   * @brief Loads the map, (populates the placeHolderMap) from a file.
   *
   * @param str
   * @param num_rows
   */
  void loadMapFromString(const std::string str, int num_rows);

 private:
  std::shared_ptr<sf::RenderWindow> window_;
  UIManager& uiManager_;
  sf::Font font_;

  int num_rows_;
  TileType selectedType_;
  std::vector<Tile> placeHolderMap;
  std::vector<Tile> highlightedTiles_;
};

#endif  // SRC_MAPEDITOR_HPP_
