#include <cstdlib>
#include "include/symbol_table.hpp"

using namespace std;


int main (int argc, char *argv[]) {
  auto s = SymbolTable<string>("foo");
  s.store("foo");
  s.store("bar");

  return 0;
}


