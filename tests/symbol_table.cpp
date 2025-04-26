#include "../include/symbol_table.hpp"
#include <doctest/doctest.h>
#include <vector>

TEST_CASE("Symbol Table"){
  auto s = SymbolTable<string>("foo");
  s.store("foo");
  s.store("bar");
  CHECK(s.lookup("foo") == true);
  CHECK(s.lookup("bar") == true);

  SUBCASE("Integer") {
  auto i = SymbolTable<int>(1);
    CHECK(i.store(1) == false);
    CHECK(i.store(2) == true);
    CHECK(i.lookup(1) == true);
    CHECK(i.lookup(2) == true);
  }
  
  SUBCASE("Const") {
    auto const s1 = SymbolTable<string>("foo");
    CHECK(s1.lookup("foo") == true);
    CHECK(s1.lookup("bar") == false);
  }

  SUBCASE("With Starting Vector") {
    vector<string> v = {"foo"};
    auto s2 = SymbolTable<string>(v);
    CHECK(s2.lookup("foo") == true);
    CHECK(s2.lookup("bar") == false);
    CHECK(s2.store("bar") == true);
    CHECK(s2.lookup("bar") == true);
  }
  
  SUBCASE("Inherit SymbolTable") {
    auto s3 = SymbolTable<string>(s);
    CHECK(s3.lookup("foo"));
    CHECK(s3.lookup("bar"));
  }
}
