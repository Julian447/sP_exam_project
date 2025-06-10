#include <symbol_table.hpp>
#include <doctest/doctest.h>
#include <stdexcept>
#include <string>
#include <vector>

TEST_CASE("SymbolTable basic operations") {
    SymbolTable<std::string, int> st;
    CHECK(st.store("foo", 1));
    CHECK_FALSE(st.store("foo", 2)); // duplicate key
    CHECK(st.lookup("foo") == 1);

    CHECK(st.store("bar", 3));
    CHECK(st.lookup("bar") == 3);

    st.increment("foo");
    CHECK(st.lookup("foo") == 2);

    st.decrement("foo");
    CHECK(st.lookup("foo") == 1);
}

TEST_CASE("SymbolTable lookup failure") {
    SymbolTable<std::string, int> st;
    st.store("foo", 1);
    CHECK_THROWS_AS(st.lookup("bar"), std::out_of_range);
}

TEST_CASE("SymbolTable pretty print") {
    SymbolTable<std::string, int> st;
    st.store("foo", 1);
    st.store("bar", 2);
    // Just check that print() does not throw
    CHECK_NOTHROW(st.print());
}
