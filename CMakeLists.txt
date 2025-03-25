cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(ChessC)

# Definindo o padrão de C
set(CMAKE_C_STANDARD 11)

# Diretório de inclusão (header files)
include_directories(include)

# Definir os arquivos fontes (todos os .c em src)
file(GLOB SOURCES "src/*.c")

# Definir o nome do executável
add_executable(chess ${SOURCES})

# Especificar os diretórios de compilação
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

