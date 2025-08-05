cmake_minimum_required (VERSION 3.22)

project(URViewer)

# Generate compile_commands.json for LSP's
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ Standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# URViewer build options
option(FETCH_RAYLIB "Fetch raylib using FetchContent" ON)
option(FETCH_UR_RTDE "Fetch ur_rtde using FetchContent" ON)

# FetchContent for getting dependencies
if (FETCH_RAYLIB OR FETCH_UR_RTDE)
    include(FetchContent)
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
    set(FETCHCONTENT_QUIET OFF)
endif()

# Clone raylib from GitHub, or find it on the system
if (FETCH_RAYLIB)
    message(STATUS "Cloning raylib using FetchContent.")
    FetchContent_Declare(raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG         5.5
        GIT_SHALLOW     TRUE
        GIT_PROGRESS    TRUE
    )
    FetchContent_MakeAvailable(raylib)
else()
    message(STATUS "Attempting to find raylib on the system.")
    find_package(raylib REQUIRED STATIC CONFIG HINTS ${raylib_DIR})
    if(raylib_FOUND)
        message(STATUS "raylib found!")
        message(STATUS "raylib DIR: ${raylib_DIR}")
    else()
        message(FATAL_ERROR "raylib not found on your system. Please install it or enable -DFETCH_RAYLIB=ON.")
    endif()
endif()

# Clone ur_rtde from GitLab, or find it on the system
if (FETCH_UR_RTDE)
    message(STATUS "Cloning ur_rtde using FetchContent.")
    set(EXAMPLES OFF CACHE BOOL "" FORCE)
    set(PYTHON_BINDINGS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(ur_rtde
        GIT_REPOSITORY https://gitlab.com/sdurobotics/ur_rtde.git
        GIT_TAG         v1.6.0
        GIT_SHALLOW     TRUE
        GIT_PROGRESS    TRUE
        GIT_SUBMODULES  ""
    )
    FetchContent_MakeAvailable(ur_rtde)
else()
    message(STATUS "Attempting to find ur_rtde on the system.")
    find_package(ur_rtde REQUIRED STATIC CONFIG HINTS ${ur_rtde_DIR})
    if(ur_rtde_FOUND)
        message(STATUS "ur_rtde found!")
        message(STATUS "ur_rtde DIR: ${ur_rtde_DIR}")
    else()
        message(FATAL_ERROR "ur_rtde not found on your system. Please install it or enable -DFETCH_UR_RTDE=ON.")
    endif()
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
