cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(ProyectoSO)

# Estándar de C++
set(CMAKE_CXX_STANDARD 17)

# Ruta de Raylib (IMPORTANTE)
set(RAYLIB_PATH "C:/raylib")

# Incluir headers
include_directories(${RAYLIB_PATH}/raylib/src)

# Linkear librerías
link_directories(${RAYLIB_PATH}/raylib/src)

# Buscar todos los .cpp del proyecto
file(GLOB_RECURSE SOURCES src/*.cpp)

# Crear ejecutable
add_executable(ProyectoSO ${SOURCES})

# Librerías necesarias (Windows + Raylib)
target_link_libraries(ProyectoSO
    raylib
    opengl32
    gdi32
    winmm
)