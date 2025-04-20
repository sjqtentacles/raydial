# Test configuration
find_package(cmocka REQUIRED)

# Add cmocka include directories
include_directories(${CMOCKA_INCLUDE_DIR})

# Build tests with cmocka
add_executable(raydial_tests raydial_tests_cmocka.c)
target_link_libraries(raydial_tests PRIVATE 
    raydial
    ${CMOCKA_LIBRARIES}
)

# Install tests
install(TARGETS 
    raydial_tests
    DESTINATION ${CMAKE_INSTALL_BINDIR}
) 