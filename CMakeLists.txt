cmake_minimum_required(VERSION 3.15)
project(Weather)

# Set C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include header directories
include_directories(${CMAKE_SOURCE_DIR}/Source)
include_directories(${CMAKE_SOURCE_DIR}/Include)
include_directories(${CMAKE_SOURCE_DIR}/Include/imgui)

# Add application sources
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/Source/*.cpp")

# Add executable
add_executable(Weather ${SOURCES})

# Find and link with SFML and other necessary libraries
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
find_package(OpenGL REQUIRED)
find_package(cpprestsdk REQUIRED)

target_link_libraries(Weather sfml-graphics sfml-window sfml-system ${OPENGL_LIBRARIES} cpprestsdk::cpprest)

# Copy resources to build folder
file(COPY ${CMAKE_SOURCE_DIR}/Resources DESTINATION ${CMAKE_BINARY_DIR})

# Copy resources to executable directory
add_custom_command(
    TARGET Weather POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Resources" "$<TARGET_FILE_DIR:Weather>/Resources"
)

# Windows-specific configuration
if(WIN32)
    target_link_libraries(Weather opengl32 ws2_32 bcrypt)
endif()
