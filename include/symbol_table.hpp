#include <cxxabi.h>  // For abi::__cxa_demangle
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>

using namespace std;

template<typename T>
class SymbolTable {
  vector<T> table;

  public:
    SymbolTable(){}
    SymbolTable(const T& t){
      store(t);
    }
    ~SymbolTable(){}
    
    bool lookup(const T& t) const {
      auto exist = std::find_if(table.begin(), table.end(), [t](const auto& p) {return t == p;}); 
      return exist != table.end();
    }

    void store(const T& t) {
      try { 
        bool exists = lookup(t);
        if (!exists) {
          table.push_back(t);
          cout << "Stored object in table" << endl;
        }
        else {
          string s = "object already exist";
          throw s;
        }
      } catch (const string& s) { // catch by const reference
        cout << "Exception caught: " << s << endl;
      }
    }
};

