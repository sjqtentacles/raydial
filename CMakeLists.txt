cmake_minimum_required(VERSION 3.10)
project(raydial VERSION 0.1.0 LANGUAGES C)

# Options
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" OFF)

# Find raylib package
find_package(raylib REQUIRED)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Library configuration
set(LIBRARY_NAME raydial)
set(SOURCES 
    src/raydial.c
)
set(HEADERS 
    include/raydial.h
)

# Create library
add_library(${LIBRARY_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${LIBRARY_NAME} PUBLIC include)
target_link_libraries(${LIBRARY_NAME} PUBLIC raylib)

# Install configuration
include(GNUInstallDirs)
install(TARGETS ${LIBRARY_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Examples
if(BUILD_EXAMPLES AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples)
    add_subdirectory(examples)
endif()

# Enable testing
enable_testing()

# Tests
if(BUILD_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    add_subdirectory(tests)
endif() 