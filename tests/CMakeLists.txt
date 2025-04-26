
add_executable(symbol_table_test symbol_table.cpp)
target_link_libraries(symbol_table_test PRIVATE symbol_table doctest::doctest_with_main)
add_test(NAME symbol_table_test COMMAND symbol_table_test)


set_tests_properties(symbol_table_test PROPERTIES RUN_SERIAL TRUE)
