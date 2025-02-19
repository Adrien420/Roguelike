# Trouve la bibliothèque SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

#Ajout des fichiers source
set(SRC AssetManager.cpp GameManager.cpp main.cpp Map.cpp)

# Crée l'exécutable à partir du fichier main.cpp
add_executable(${EXEC} ${SRC})

# Lie l'exécutable avec la bibliothèque SDL2
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# Installation de l'exécutable
install(TARGETS ${EXEC} RUNTIME DESTINATION bin)
