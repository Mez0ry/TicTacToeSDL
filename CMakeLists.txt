cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

include(FindPackageHandleStandardArgs)

project(TicTacToe LANGUAGES CXX VERSION 0.18.1)

set(ProjectName TicTacToe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)


set(TicTacToe_HEADERS 
    src/Board.hpp
    src/Entity.hpp
    src/Exit.hpp
    src/Game.hpp
    src/GameScene.hpp
    src/Menu.hpp
    src/Player.hpp
    src/Playing.hpp
    src/Sign.hpp
)
set(TicTacToe_SOURCES 
    src/main.cpp
    src/Board.cpp
    src/Exit.cpp
    src/Game.cpp
    src/Menu.cpp
    src/Player.cpp
    src/Playing.cpp
)
                      
add_executable(${ProjectName} ${TicTacToe_HEADERS} ${TicTacToe_SOURCES})

# add_custom_command(TARGET ${ProjectName} PRE_BUILD
#                 COMMAND ${CMAKE_COMMAND} -E copy_directory
#                     ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:${ProjectName}>)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/Stellar)
target_include_directories(${ProjectName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Stellar/Stellar)
target_link_libraries(${ProjectName} Stellar)