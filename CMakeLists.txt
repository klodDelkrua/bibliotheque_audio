cmake_minimum_required(VERSION 4.0)
project(bibliotheque_audio)

set(CMAKE_CXX_STANDARD 23)

# ==========================================================
# 1. PARAMÈTRES ET STRUCTURE DE FICHIERS
# ==========================================================

# Permet au compilateur de trouver les fichiers d'en-tête (.h) dans le dossier 'include/'
# sans avoir à modifier les chemins d'accès dans le code C++ (ex: #include "song.h").
include_directories(include)

# Définit la liste de tous les fichiers sources (.cpp) situés dans le dossier src/.
# Le chemin doit être mis à jour pour pointer vers les nouveaux emplacements.
file(GLOB_RECURSE SOURCES "src/*.cpp")

# ==========================================================
# 2. TROUVER ET LIER LIBPQXX (INCHANGÉ)
# ==========================================================

# A. Cherche le programme 'pkg-config'
find_package(PkgConfig REQUIRED)

# B. Demande à pkg-config de trouver les informations pour libpqxx
pkg_check_modules(PQXX REQUIRED libpqxx)

# C. Utilise les informations trouvées pour compiler (includes)
# Bien que nous utilisions 'include_directories(include)' plus haut,
# cette ligne est nécessaire pour les en-têtes de pqxx.
include_directories(${PQXX_INCLUDE_DIRS})

# ==========================================================
# 3. CRÉATION DE L'EXÉCUTABLE
# ==========================================================

# D. Définit l'exécutable en utilisant la variable ${SOURCES} qui contient tous les .cpp
add_executable(bibliotheque_audio ${SOURCES})

# E. L'étape CRITIQUE : Dit au linker d'inclure le code de libpqxx
target_link_libraries(bibliotheque_audio ${PQXX_LIBRARIES})

# ==========================================================