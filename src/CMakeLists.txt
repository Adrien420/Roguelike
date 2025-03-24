find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(/usr/include/SDL2)
include_directories(${SDL2_MIXER_INCLUDE_DIRS})

# Ajout des fichiers source
set(SRC AssetManager.cpp GameManager.cpp main.cpp Map.cpp CardsManager.cpp Bonus.cpp SoundManager.cpp)  # Ajout de SoundManager.cpp

# Crée l'exécutable à partir du fichier main.cpp
add_executable(${EXEC} ${SRC})

# Lie l'exécutable avec les bibliothèques SDL2
target_link_libraries(${EXEC} ${SDL2_LIBRARIES} SDL2_image SDL2_ttf SDL2_mixer)
