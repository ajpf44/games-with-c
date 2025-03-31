# Defina a versão mínima do CMake e o projeto
cmake_minimum_required(VERSION 3.10)
project(campo_minado)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find GTK package
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

# Adicione os diretórios de include para GTK e outros cabeçalhos
include_directories(${GTK_INCLUDE_DIRS})
include_directories(include)

# Adicione os arquivos de código-fonte
file(GLOB SOURCES src/*.c)

# Crie o executável, passando o arquivo main.c e os arquivos da src
add_executable(campo_minado ${SOURCES})

# Compiler flags
add_compile_options(${GTK_CFLAGS_OTHER})

# Link libraries
target_link_libraries(campo_minado ${GTK_LIBRARIES})

