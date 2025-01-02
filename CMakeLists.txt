cmake_minimum_required(VERSION 3.28)
project(StrategyGame LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(MAPS_DIR ${CMAKE_SOURCE_DIR}/src/maps)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(
    main 
    src/building.hpp
    src/building.cpp
    src/gamestate.hpp 
    src/gamestate.cpp 
    src/main.cpp 
    src/player.hpp 
    src/player.cpp 
    src/playerAI.hpp
    src/playerAI.cpp
    src/tile.hpp 
    src/tile.cpp 
    src/ui.hpp 
    src/ui.cpp
    src/utils.hpp
    src/utils.cpp
    src/army.hpp
    src/army.cpp
    src/townhall.hpp
    src/townhall.cpp
    src/selectable.hpp
    src/uimanager.hpp
    src/uimanager.cpp
    src/ui/mapeditor.cpp
    src/ui/mapeditor.hpp
    src/ui/mainmenu.hpp
    src/ui/mainmenu.cpp
    src/ui/victory_screen.cpp
    src/ui/victory_screen.cpp
)

target_link_libraries(main PRIVATE sfml-graphics)
target_compile_features(main PRIVATE cxx_std_17)
file(COPY ${MAPS_DIR} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/src/HackNerdFont-Regular.ttf DESTINATION ${CMAKE_BINARY_DIR})

if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()

# Add testing executable
enable_testing()

add_executable(
    player_tests
    tests/test_Player.cpp
    src/player.hpp
    src/player.cpp
)

add_executable(
    army_tests
    tests/test_Army.cpp
    src/player.hpp
    src/player.cpp
    src/army.hpp
    src/army.cpp
)

add_executable(
    tile_tests
    tests/test_Tile.cpp
    src/player.hpp
    src/player.cpp
    src/tile.hpp
    src/tile.cpp
)

add_executable(
    townhall_tests
    tests/test_Townhall.cpp
    src/player.hpp
    src/player.cpp
    src/army.hpp
    src/army.cpp
    src/townhall.hpp
    src/townhall.cpp
)

add_executable(
    building_tests
    tests/test_Building.cpp
    src/building.hpp
    src/building.cpp
    src/player.hpp
    src/player.cpp
)

target_link_libraries(player_tests PRIVATE sfml-graphics)
target_compile_features(player_tests PRIVATE cxx_std_17)
target_include_directories(player_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(army_tests PRIVATE sfml-graphics)
target_compile_features(army_tests PRIVATE cxx_std_17)
target_include_directories(army_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(tile_tests PRIVATE sfml-graphics)
target_compile_features(tile_tests PRIVATE cxx_std_17)
target_include_directories(tile_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(townhall_tests PRIVATE sfml-graphics)
target_compile_features(townhall_tests PRIVATE cxx_std_17)
target_include_directories(townhall_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(building_tests PRIVATE sfml-graphics)
target_compile_features(building_tests PRIVATE cxx_std_17)
target_include_directories(building_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Add a CTest
add_test(NAME PlayerTests COMMAND player_tests)
add_test(NAME ArmyTests COMMAND army_tests)
add_test(NAME TileTests COMMAND tile_tests)
add_test(NAME TownhallTests COMMAND townhall_tests)
add_test(NAME BuildingTests COMMAND building_tests)