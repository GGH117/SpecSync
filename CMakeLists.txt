cmake_minimum_required(VERSION 3.15)
project(SpecSync)

set(CMAKE_CXX_STANDARD 20)

# 1. Define Core Sources
set(SPECSYNC_SOURCES
    src/Telemetry/Telemetry.cpp
    src/OptimizationEngine/OptimizationEngine.cpp
    src/SettingsRegistry/SettingsRegistry.cpp
)

# 2. Add Linux-Specific Sources if not on Windows
if(NOT WIN32)
    list(APPEND SPECSYNC_SOURCES src/Telemetry/LinuxShim.cpp)
    add_definitions(-DPLATFORM_LINUX)
endif()

# 3. Build the Library
add_library(SpecSync STATIC ${SPECSYNC_SOURCES})

# 4. Link Dependencies
find_package(Threads REQUIRED)
target_link_libraries(SpecSync PRIVATE Threads::Threads) # Threading required on all OS

if(WIN32)
    target_link_libraries(SpecSync PRIVATE dxgi.lib pdh.lib)
endif()

# 5. Build Demo
add_executable(SpecSyncDemo demo/main.cpp)
target_link_libraries(SpecSyncDemo PRIVATE SpecSync)