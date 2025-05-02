#include <memory>
#include <string>

// #include <symbol_table.hpp>
#include <reactant.hpp>


using namespace std;

namespace stocasthic {

  class Vessel {
    string title;
    shared_ptr<SymbolTable<string, int>> table_ptr;
  public: 
    // Vessel(string s) : title(s) {}
    void environment() {
      table_ptr = shared_ptr<SymbolTable<string, int>>(new SymbolTable<string, int>());
    }

    template<typename K, typename V>
    Reactant<K,V> add(K a, V b) {
      table_ptr->store(a, b);
      auto r = Reactant<K, V>{a,b,table_ptr};
      return r;
    }
  };
}



