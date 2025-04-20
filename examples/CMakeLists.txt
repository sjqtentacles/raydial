# Example configuration
add_executable(0_basic_example 0_basic_example.c)
target_link_libraries(0_basic_example PRIVATE raydial)

add_executable(1_counter_example 1_counter_example.c)
target_link_libraries(1_counter_example PRIVATE raydial)

add_executable(2_dialogue_tree_example 2_dialogue_tree_example.c)
target_link_libraries(2_dialogue_tree_example PRIVATE raydial)

add_executable(3_animated_text_example 3_animated_text_example.c)
target_link_libraries(3_animated_text_example PRIVATE raydial)

# Install examples
install(TARGETS 
    0_basic_example
    1_counter_example
    2_dialogue_tree_example
    3_animated_text_example
    DESTINATION ${CMAKE_INSTALL_BINDIR}
) 