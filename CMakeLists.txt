cmake_minimum_required(VERSION 3.15)
project(SpecSync VERSION 1.0.0 LANGUAGES CXX)

# Standard C++20 for performance features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Define the Static Library Target
add_library(SpecSync STATIC
    src/HardwareScanner/HardwareScanner.cpp
    src/Telemetry/Telemetry.cpp
    src/OptimizationEngine/OptimizationEngine.cpp
)

# Configure Include Directories
# PUBLIC: Anyone linking to SpecSync needs the 'include' folder
# PRIVATE: Only the library needs its own internal 'src' folders
target_include_directories(SpecSync PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(SpecSync PRIVATE 
    src/HardwareScanner
    src/Telemetry
    src/OptimizationEngine
)

# Link OS-Specific Graphics APIs
if(WIN32)
    target_link_libraries(SpecSync PRIVATE dxgi.lib)
    target_compile_definitions(SpecSync PRIVATE _WIN32)
else()
    # Support for Vulkan (Steam Deck / Linux)
    find_package(Vulkan REQUIRED)
    target_link_libraries(SpecSync PRIVATE Vulkan::Vulkan)
endif()

# Professional Naming
set_target_properties(SpecSync PROPERTIES
    DEBUG_POSTFIX "d"       # Produces SpecSyncd.lib in debug
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# DEMO EXECUTABLE
# Builds an executable named SpecSyncDemo
add_executable(SpecSyncDemo demo/main.cpp)

# Link the executable to SpecSync static library
target_link_libraries(SpecSyncDemo PRIVATE SpecSync)