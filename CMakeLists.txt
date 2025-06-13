cmake_minimum_required (VERSION 3.22)

project(URViewer)

# Generate compile_commands.json for LSP's
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ Standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Local cmake projects might be built here
list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.local")

# # Built binaries in build/bin, libraries in build/lib
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(raylib REQUIRED STATIC CONFIG)
if(raylib_FOUND)
    message(STATUS "raylib found!")
    message(STATUS "raylib DIR: ${raylib_DIR}")
endif()

add_executable(URViewer src/main.cpp)
target_include_directories(URViewer
  PRIVATE src
)

target_link_libraries(URViewer
  PRIVATE raylib 
)
