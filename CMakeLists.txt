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
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

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
    set(BUILD_STATIC ON CACHE BOOL "" FORCE)
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

# Fetch pace
message(STATUS "Cloning pace using FetchContent.")
FetchContent_Declare(pace
    GIT_REPOSITORY https://github.com/nmarks99/pace.git
    GIT_TAG        master
    GIT_SHALLOW    FALSE
)
FetchContent_MakeAvailable(pace)

# --- EPICS Base ---------------------------------------------------------------
# Get path to EPICS base from environment variable or -DEPICS_BASE
if(NOT DEFINED EPICS_BASE OR EPICS_BASE STREQUAL "")
    set(EPICS_BASE $ENV{EPICS_BASE})
endif()
if(NOT EPICS_BASE OR EPICS_BASE STREQUAL "")
    message(FATAL_ERROR "Missing required variable 'EPICS_BASE'. Provide it via -DEPICS_BASE=... or set the EPICS_BASE environment variable.")
endif()
if(NOT IS_DIRECTORY "${EPICS_BASE}")
    message(FATAL_ERROR "Specified EPICS_BASE does not exist: ${EPICS_BASE}")
endif()

# Put it into the cache so it’s visible in GUI tools or later CMake runs
set(EPICS_BASE "${EPICS_BASE}" CACHE STRING "Path to EPICS base")
message(STATUS "Found EPICS_BASE: ${EPICS_BASE}")

# Get EPICS host architecture
set(EPICS_HOST_ARCH "$ENV{EPICS_HOST_ARCH}" CACHE STRING "Host architecture (e.g. linux-x86_64")
if (NOT EPICS_HOST_ARCH)
    message(FATAL_ERROR "'EPICS_HOST_ARCH' not defined")
endif()
message(STATUS "EPICS_HOST_ARCH: ${EPICS_HOST_ARCH}")

set(EPICS_LIB_DIR "${EPICS_BASE}/lib/${EPICS_HOST_ARCH}")
foreach(LIB ca Com)
    find_library(EPICS_${LIB} ${LIB}
        PATHS ${EPICS_LIB_DIR}
        NO_DEFAULT_PATH
        REQUIRED
    )
endforeach()

set(EPICS_BASE_INCLUDES
    ${EPICS_BASE}/include
    ${EPICS_BASE}/include/compiler/gcc
    ${EPICS_BASE}/include/compiler/clang
    ${EPICS_BASE}/include/pv
    ${EPICS_BASE}/include/pva
    ${EPICS_BASE}/include/os/Linux
    ${EPICS_BASE}/include/os/Darwin
)

# --- PVXS ---------------------------------------------------------------------
if(NOT DEFINED PVXS OR PVXS STREQUAL "")
    set(PVXS $ENV{PVXS})
endif()
if(NOT PVXS OR PVXS STREQUAL "")
    message(FATAL_ERROR "PVXS not set. Pass -DPVXS=... or set the PVXS environment variable.")
endif()
if(NOT IS_DIRECTORY "${PVXS}")
    message(FATAL_ERROR "Specified PVXS does not exist: ${PVXS}")
endif()
set(PVXS "${PVXS}" CACHE PATH "Path to PVXS")
message(STATUS "Found PVXS: ${PVXS}")

set(PVXS_LIB_DIR "${PVXS}/lib/${EPICS_HOST_ARCH}")
find_library(PVXS_LIBRARY pvxs
    PATHS ${PVXS_LIB_DIR}
    NO_DEFAULT_PATH
    REQUIRED
)
set(PVXS_INCLUDES "${PVXS}/include")
# ------------------------------------------------------------------------------

# Define a preprocessor macro for the resources directory
# add_compile_definitions(URVIEWER_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources")
add_compile_definitions(URVIEWER_RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/share/URViewer/resources")

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
    PRIVATE ${EPICS_BASE_INCLUDES}
    PRIVATE ${PVXS_INCLUDES}
)

target_link_libraries(URViewer
    PRIVATE raylib
    PRIVATE ur_rtde::rtde
    PRIVATE pace::pace
    PRIVATE ${EPICS_ca}
    PRIVATE ${EPICS_Com}
    PRIVATE ${PVXS_LIBRARY}
)
#####################################################

set_target_properties(URViewer PROPERTIES
    INSTALL_RPATH "${EPICS_LIB_DIR};${PVXS_LIB_DIR}"
    BUILD_WITH_INSTALL_RPATH FALSE
)

# Install the binary into ${CMAKE_INSTALL_PREFIX}/bin
install(TARGETS URViewer
    RUNTIME DESTINATION bin
)

# Install resources into ${CMAKE_INSTALL_PREFIX}/share/URViewer/resources
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources
    DESTINATION share/URViewer
)
