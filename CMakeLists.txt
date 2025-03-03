# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(Roguelike)

# Spécifie le standard C++ à utiliser
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Utilisation de AddressSanitizer, qui est un outil de détection d'erreurs de mémoire
# if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
#     set(SANITIZER_FLAGS "-fsanitize=address")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS} -g")
#     set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${SANITIZER_FLAGS}")
#     # Option pour ignorer les leaks des bibliothèques système
#     set(ENV{ASAN_OPTIONS} "detect_leaks=1:fast_unwind_on_malloc=0:malloc_context_size=0:detect_leaks_experimental=1:suppressions=asan_ignore_libs.txt")
# endif()

# Définit le nom de l'exécutable
set(EXEC Game)

# Définit les dossiers
set(SRC ${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${SRC})