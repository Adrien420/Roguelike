# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(Roguelike)

# Spécifie le standard C++ à utiliser
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouve la bibliothèque SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Crée l'exécutable à partir du fichier main.cpp
add_executable(${PROJECT_NAME} main.cpp)

# Lie l'exécutable avec la bibliothèque SDL2
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
