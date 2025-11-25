cmake_minimum_required(VERSION 4.0)
project(bibliotheque_audio)

set(CMAKE_CXX_STANDARD 23)

# ==========================================================
# 1. TROUVER ET LIER LIBPQXX
# ==========================================================

# A. Cherche le programme 'pkg-config' (utilisé pour trouver les librairies)
find_package(PkgConfig REQUIRED)

# B. Demande à pkg-config de trouver les informations pour libpqxx
#    (Inclut les chemins d'en-tête et les noms des librairies)
pkg_check_modules(PQXX REQUIRED libpqxx)

# C. Utilise les informations trouvées pour compiler (includes)
include_directories(${PQXX_INCLUDE_DIRS})

# D. Définit l'exécutable
add_executable(bibliotheque_audio
        main.cpp
        audio_player.cpp
        audio_player.h
        user_interface.cpp
        user_interface.h
        authManager.cpp
        authManager.h
        user_account.cpp
        user_account.h
        song.cpp
        song.h
        album.cpp
        album.h
        playlist.cpp
        playlist.h
)

# E. L'étape CRITIQUE : Dit au linker d'inclure le code de libpqxx
target_link_libraries(bibliotheque_audio ${PQXX_LIBRARIES})

# ==========================================================