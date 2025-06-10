
add_executable(symbol_table_test symbol_table.cpp)
add_executable(reaction_test reaction.cpp)

target_link_libraries(symbol_table_test
    PRIVATE
      vessel 
      doctest::doctest_with_main
)

target_link_libraries(reaction_test
    PRIVATE
      vessel 
      doctest::doctest_with_main
)

add_test(NAME symbol_table_test COMMAND symbol_table_test)
add_test(NAME reaction_test COMMAND reaction_test)

set_tests_properties(symbol_table_test PROPERTIES RUN_SERIAL TRUE)
set_tests_properties(reaction_test PROPERTIES RUN_SERIAL TRUE)
