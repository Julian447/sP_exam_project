#include <memory>
#include <string>

// #include <symbol_table.hpp>
#include <reaction.hpp>


using namespace std;

namespace stochastic {

  class Vessel {
    string title;
    unique_ptr<SymbolTable<string, float>> table_ptr;
  public: 
    Vessel(string s) : title(s) {
      table_ptr = unique_ptr<SymbolTable<string, float>>(new SymbolTable<string, float>());
    }
    int environment() {return 0; } //idk what to do with this

    template<typename K, typename V>
    Reaction<K,V> add(K a, V b) {
      table_ptr->store(a, b);
      auto r = Reaction<K, V>{a,b};
      return r;
    }
    template<typename K, typename V>
    void add(const Reaction<K,V>&& r) {

      for (auto& i : r.input_keys){
        table_ptr->decrement(i);
        // cout << "[Input] " << i << " " << table_ptr->lookup(i) << endl;
      }
      for (auto& i : r.product_keys){
        table_ptr->increment(i);
        // cout << "[Product] " << i << " " << table_ptr->lookup(i) << endl;
      }
      // cout << endl;
      // table_ptr->print();
      // cout << endl;
    }
    void print_table(){
      table_ptr->print();
    }
  };
}



