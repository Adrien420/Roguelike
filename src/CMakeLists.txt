# Trouve la bibliothèque SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(/usr/include/SDL2)

#Ajout des fichiers source
set(SRC AssetManager.cpp GameManager.cpp main.cpp Map.cpp CardsManager.cpp)

# Crée l'exécutable à partir du fichier main.cpp
add_executable(${EXEC} ${SRC})

# Ajoute la définition DEBUG uniquement si on compile en Debug
target_compile_definitions(${EXEC} PRIVATE $<$<CONFIG:Debug>:DEBUG>)

# Lie l'exécutable avec la bibliothèque SDL2
target_link_libraries(${EXEC} ${SDL2_LIBRARIES} SDL2_image SDL2_ttf)

# Installation de l'exécutable
install(TARGETS ${EXEC} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
