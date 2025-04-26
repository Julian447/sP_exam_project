#include <cstdlib>
#include <stdexcept>
#include "include/symbol_table.hpp"

using namespace std;


int main (int argc, char *argv[]) {
  auto s = SymbolTable<string,int>("foo",1);
  try {
  cout << "found 'foo': " << s.lookup("foo") << endl;
  cout << "found 'bar': " << s.lookup("bar") << endl;
  } catch (const out_of_range& e) {
    cerr << "Caught an exception in lookup: " << e.what() << endl;
  }
  cout << "stored 'foo': " << s.store("foo",2) << endl;
  cout << "stored 'bar': " << s.store("bar",3) << endl;
  cout << "found 'foo': " << s.lookup("foo") << endl;
  cout << "found 'bar': " << s.lookup("bar") << endl;
  cout << endl;

  return 0;
}


