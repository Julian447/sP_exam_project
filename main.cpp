#include <cstdlib>
#include "include/symbol_table.hpp"

using namespace std;


int main (int argc, char *argv[]) {
  auto s = SymbolTable<string>("foo");
  s.store("foo");
  s.store("bar");
  cout << "found 'foo': " << s.lookup("foo") << endl;
  cout << "found 'bar': " << s.lookup("bar") << endl;
  cout << endl;

  auto const s1 = SymbolTable<string>("foo");
  // s1.store("foo");
  // s1.store("bar");
  cout << "found 'foo': " << s1.lookup("foo") << endl;
  cout << "found 'bar': " << s1.lookup("bar") << endl;
  cout << endl;
  
  vector<string> v = {"foo"};
  auto s2 = SymbolTable<string>(v);
  s2.store("foo");
  s2.store("bar");
  cout << "found 'foo': " << s2.lookup("foo") << endl;
  cout << "found 'bar': " << s2.lookup("bar") << endl;
  cout << endl;

  auto s3 = SymbolTable<string>(s);
  cout << "found 'foo': " << s3.lookup("foo") << endl;
  cout << "found 'bar': " << s3.lookup("bar") << endl;
  cout << endl;

  return 0;
}


