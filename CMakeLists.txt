cmake_minimum_required(VERSION 3.10)

# Project Information
project(SunWukong_vs_CaptainAmerica VERSION 1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags for warnings and debugging
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Build types: Debug and Release
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

# Add source files
file(GLOB SOURCES src/*.cpp)

# Include directories
include_directories(include)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set SFML directory based on platform
if(WIN32)
    # Windows platform
    set(SFML_DIR "C:/path/to/SFML/lib/cmake/SFML")
elseif(APPLE)
    # macOS platform
    set(SFML_DIR "/usr/local/Cellar/sfml/2.6.2/lib/cmake/SFML")
endif()

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
if(SFML_FOUND)
    target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
else()
    message(FATAL_ERROR "SFML library not found. Please set SFML_DIR correctly.")
endif()

# Debugging CPack variables
message(STATUS "Setting CPack variables...")
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Project Version: ${PROJECT_VERSION}")

# CPack configurations for packaging the project
# The following settings define project details such as name, version, and vendor information.
set(CPACK_PROJECT_NAME "SunWukong_vs_CaptainAmerica")
set(CPACK_PACKAGE_NAME "SunWukong_vs_CaptainAmerica")
set(CPACK_PACKAGE_VERSION "1.0")
set(CPACK_PACKAGE_VENDOR "Xue (Aria) Wang")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A fighting game featuring Sun Wukong and Captain America.")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_GENERATOR "DragNDrop")  
set(CPACK_PACKAGE_INSTALL_DIRECTORY "SunWukong_vs_CaptainAmerica Installer")

# macOS-specific configurations
if(APPLE)
    set(CPACK_DMG_VOLUME_NAME "${CPACK_PACKAGE_NAME}")
endif()


# Include CPack module 
include(CPack)

# Enable sanitizers in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined")
endif()

# Cross-compilation example for ARM architecture
# The following settings are used to configure cross-compilation. 
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Specify the target processor (e.g., ARM)
    set(CMAKE_SYSTEM_PROCESSOR arm)
    # Use a custom toolchain file for cross-compilation
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchain.cmake")
endif()

if(NOT SFML_FOUND)
    add_subdirectory(vendor/SFML)
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
endif()



