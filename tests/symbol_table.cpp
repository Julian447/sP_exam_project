#include "../include/symbol_table.hpp"
#include <doctest/doctest.h>
#include <stdexcept>
#include <string>
#include <vector>

TEST_CASE("Symbol Table"){
  auto s = SymbolTable<string,int>("foo",1);
  CHECK(s.lookup("foo") == 1);
  CHECK(s.store("foo",2) == false);
  CHECK(s.store("bar",3) == true);
  CHECK(s.lookup("foo") == 1);
  CHECK(s.lookup("bar") == 3);
  
  SUBCASE("Const") {
    auto const s1 = SymbolTable<string,int>("foo",1);
    CHECK(s1.lookup("foo"));
    CHECK_THROWS_AS(s1.lookup("bar"), out_of_range);
  }

  SUBCASE("With Starting Map") {
    map<string,int> um = {{"foo",1}};
    auto s2 = SymbolTable<string,int>(um);
    CHECK(s2.lookup("foo"));
    CHECK_THROWS_AS(s2.lookup("bar"), out_of_range);

    CHECK(s2.store("bar", 1) == true);
    CHECK(s2.lookup("bar"));
  }

  SUBCASE("Inherit SymbolTable") {
    auto s3 = SymbolTable<string,int>(s);
    CHECK(s3.lookup("foo"));
    CHECK(s3.lookup("bar"));
  }
}
