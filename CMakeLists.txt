cmake_minimum_required(VERSION 3.10)
project(Nonogram_Parallel_Solver)

set(CMAKE_CXX_STANDARD 17)

# Adiciona todos os arquivos fonte dos diretórios src e utils
file(GLOB SRC_FILES src/*.cpp)
file(GLOB UTIL_FILES utils/*.cpp)
file(GLOB LINE_SOLVER_FILES utils/linesolvers/*.cpp)
list(APPEND UTIL_FILES ${LINE_SOLVER_FILES})

add_executable(nonogram_solver ${SRC_FILES} ${UTIL_FILES})

# Inclui os diretórios de cabeçalhos
target_include_directories(nonogram_solver PRIVATE src utils)