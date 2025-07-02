cmake_minimum_required (VERSION 3.22)

project(URViewer)

# Generate compile_commands.json for LSP's
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ Standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib REQUIRED STATIC CONFIG HINTS ${raylib_DIR})
if(raylib_FOUND)
    message(STATUS "raylib found!")
    message(STATUS "raylib DIR: ${raylib_DIR}")
endif()

find_package(ur_rtde CONFIG REQUIRED STATIC HINTS ${ur_rtde_DIR})
if(ur_rtde_FOUND)
    message(STATUS "ur_rtde found!")
    message(STATUS "ur_rtde DIR: ${ur_rtde_DIR}")
endif()

# Define a preprocessor macro for the resources directory
add_compile_definitions(URVIEWER_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources")

#####################################################
add_executable(URViewer
    src/main.cpp
    src/rl_utils.cpp
    src/ur.cpp
    src/ui.cpp
    src/comm.cpp
)

target_include_directories(URViewer
    PRIVATE src
    PRIVATE external
)

target_link_libraries(URViewer
    PRIVATE raylib 
    PRIVATE ur_rtde::rtde
)
