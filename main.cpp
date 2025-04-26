#include <cstdlib>
#include "include/symbol_table.hpp"

using namespace std;


int main (int argc, char *argv[]) {
  auto s = SymbolTable<string,int>("foo",1);
  cout << "stored 'foo': " << s.store("foo",2) << endl;
  cout << "stored 'bar': " << s.store("bar",1) << endl;
  cout << "found 'foo': " << s.lookup("foo") << endl;
  cout << "found 'bar': " << s.lookup("bar") << endl;
  cout << endl;

  return 0;
}


