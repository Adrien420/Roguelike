# Trouve la bibliothèque SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED) 

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(/usr/include/SDL2)
include_directories(${SDL2_MIXER_INCLUDE_DIRS})

# Fichiers sources du projet
set(PROJECT_SOURCES ../src/GameManager.cpp ../src/AssetManager.cpp ../src/Map.cpp ../src/CardsManager.cpp ../src/Bonus.cpp ../src/SoundManager.cpp)

#Ajout des fichiers source
set(TESTS test_entity.cpp test_gamemanager.cpp test_assetmanager.cpp test_bonus.cpp)

# Crée l'exécutable à partir du fichier main.cpp
add_executable(Roguelike_tests ${TESTS} ${PROJECT_SOURCES})

# Inclure les headers du projet
target_include_directories(Roguelike_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Lier Boost.Test et SDL2
target_link_libraries(Roguelike_tests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${SDL2_LIBRARIES} SDL2_image SDL2_ttf SDL2_mixer glib-2.0)

# Installation de l'exécutable
install(TARGETS Roguelike_tests RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)