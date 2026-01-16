cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(smigrate)

add_executable(smigrate)
target_sources(smigrate 
    PRIVATE 
    src/main.cpp
    src/MigrationList.cpp
    src/MigrationNode.cpp
    src/SubCommandParser.cpp
    src/subcmds/NewHandler.cpp
)
