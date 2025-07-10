cmake_minimum_required(VERSION 3.10)
project(MancalaGame)

set(CMAKE_CXX_STANDARD 14)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add source files
set(SOURCES 
    src/main.cpp
    src/mancala.cpp
    src/ai.cpp
)

# Add header files
set(HEADERS
    src/mancala.h
    src/ai.h
)

# Create executable
add_executable(mancala ${SOURCES} ${HEADERS})

# Link SFML
target_link_libraries(mancala sfml-graphics sfml-window sfml-system)

# Include directories
target_include_directories(mancala PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Copy assets to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Installation
install(TARGETS mancala DESTINATION bin)
install(DIRECTORY assets DESTINATION bin)