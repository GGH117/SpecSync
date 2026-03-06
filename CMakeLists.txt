cmake_minimum_required(VERSION 3.15)
project(SpecSync)

set(CMAKE_CXX_STANDARD 20)

add_library(SpecSync STATIC
    src/Telemetry/Telemetry.cpp
    src/OptimizationEngine/OptimizationEngine.cpp
    src/SettingsRegistry/SettingsRegistry.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(SpecSync PRIVATE dxgi.lib pdh.lib Threads::Threads)

add_executable(SpecSyncDemo demo/main.cpp)
target_link_libraries(SpecSyncDemo PRIVATE SpecSync)