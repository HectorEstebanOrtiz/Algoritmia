cmake_minimum_required(VERSION 3.12)
project(PicrossSolver CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directorio raíz de MiniSAT (un nivel arriba)
set(MINISAT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)


# Permite resolver los #include "../minisat/core/Solver.h"
include_directories(${MINISAT_DIR})

add_subdirectory(competencia)

# Fuentes de MiniSAT necesarios para compilar directamente sin depender de .a/.lib externos
set(MINISAT_SOURCES
    ${MINISAT_DIR}/minisat/core/Solver.cc
    ${MINISAT_DIR}/minisat/utils/System.cc
    ${MINISAT_DIR}/minisat/utils/Options.cc
)

# Creamos el ejecutable con tu archivo main.cpp y las tripas de MiniSAT
add_executable(picross_solver main.cpp ${MINISAT_SOURCES})

# Macros requeridas por MiniSAT para evitar errores de compilación con tipos de datos
target_compile_definitions(picross_solver PRIVATE 
    __STDC_LIMIT_MACROS 
    __STDC_FORMAT_MACROS
)

target_link_libraries(picross_solver PRIVATE picross)