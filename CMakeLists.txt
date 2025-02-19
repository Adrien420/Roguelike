# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(Roguelike)

# Spécifie le standard C++ à utiliser
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Définit le nom de l'exécutable
set(EXEC Game)

# Définit les dossiers
set(SRC ${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${SRC})