# Example configuration
add_executable(0_basic_example 0_basic_example.c)
target_link_libraries(0_basic_example PRIVATE raydial)

add_executable(1_counter_example 1_counter_example.c)
target_link_libraries(1_counter_example PRIVATE raydial)

add_executable(2_dialogue_tree_example 2_dialogue_tree_example.c)
target_link_libraries(2_dialogue_tree_example PRIVATE raydial)

add_executable(3_animated_text_example 3_animated_text_example.c)
target_link_libraries(3_animated_text_example PRIVATE raydial)

add_executable(4_image_gallery_example 4_image_gallery_example.c)
target_link_libraries(4_image_gallery_example PRIVATE raydial)

add_executable(5_portrait_dialogue_example 5_portrait_dialogue_example.c)
target_link_libraries(5_portrait_dialogue_example PRIVATE raydial)

add_executable(6_styled_dialogue_example 6_styled_dialogue_example.c)
target_link_libraries(6_styled_dialogue_example PRIVATE raydial)

add_executable(generate_portraits generate_portraits.c)
target_link_libraries(generate_portraits PRIVATE raylib)

# Install examples
install(TARGETS 
    0_basic_example
    1_counter_example
    2_dialogue_tree_example
    3_animated_text_example
    4_image_gallery_example
    5_portrait_dialogue_example
    6_styled_dialogue_example
    DESTINATION ${CMAKE_INSTALL_BINDIR}
) 