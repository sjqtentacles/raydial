# Example configuration
add_executable(basic_example basic_example.c)
target_link_libraries(basic_example PRIVATE raydial)

add_executable(advanced_example advanced_example.c)
target_link_libraries(advanced_example PRIVATE raydial)

add_executable(dialogue_tree_example dialogue_tree_example.c)
target_link_libraries(dialogue_tree_example PRIVATE raydial)

# New numerically prefixed examples
add_executable(0_basic_example 0_basic_example.c)
target_link_libraries(0_basic_example PRIVATE raydial)

add_executable(1_counter_example 1_counter_example.c)
target_link_libraries(1_counter_example PRIVATE raydial)

add_executable(2_dialogue_tree_example 2_dialogue_tree_example.c)
target_link_libraries(2_dialogue_tree_example PRIVATE raydial)

# Additional examples
add_executable(4_animated_text_example 4_animated_text_example.c)
target_link_libraries(4_animated_text_example PRIVATE raydial)

# Install examples
install(TARGETS 
    basic_example 
    advanced_example 
    dialogue_tree_example 
    0_basic_example
    1_counter_example
    2_dialogue_tree_example
    4_animated_text_example
    DESTINATION ${CMAKE_INSTALL_BINDIR}
) 