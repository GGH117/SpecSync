cmake_minimum_required(VERSION 3.15)
project(SpecSync)

set(CMAKE_CXX_STANDARD 20)

# Define Core Sources
set(SPECSYNC_SOURCES
    src/Telemetry/Telemetry.cpp
    src/OptimizationEngine/OptimizationEngine.cpp
    src/SettingsRegistry/SettingsRegistry.cpp
)

# Platform Logic
if(WIN32)
    add_definitions(-DPLATFORM_WINDOWS)
else()
    list(APPEND SPECSYNC_SOURCES src/Telemetry/LinuxShim.cpp)
    add_definitions(-DPLATFORM_LINUX)
endif()

# Build Library
add_library(SpecSync STATIC ${SPECSYNC_SOURCES})

# Link Hardware APIs
if(WIN32)
    target_link_libraries(SpecSync PRIVATE dxgi.lib pdh.lib)
endif()

# FETCH PYBIND11 (Fixes the "file not found" error)
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.11.1
)
FetchContent_MakeAvailable(pybind11)

# Build the Python Bridge
pybind11_add_module(SpecSyncBridge ui/bridge/SpecSyncBridge.cpp)
target_link_libraries(SpecSyncBridge PRIVATE SpecSync)