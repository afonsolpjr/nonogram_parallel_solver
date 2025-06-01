cmake_minimum_required(VERSION 3.10)
project(Nonogram_Parallel_Solver)

set(CMAKE_CXX_STANDARD 17)

# Adiciona todos os arquivos fonte dos diretórios src e utils
file(GLOB_RECURSE ALL_SRC_FILES
    src/*.cpp
    utils/*.cpp
)

add_executable(nonogram_solver ${ALL_SRC_FILES})

# Inclui os diretórios de cabeçalhos
target_include_directories(nonogram_solver PRIVATE src utils)