#include <memory>
#include <string>

// #include <symbol_table.hpp>
#include <reaction.hpp>
#include <vector>


using namespace std;

namespace stochastic {

  template<typename K = string, typename V = float>
  class Vessel {
    string title;
  public: 
    vector<Reaction<K,V>> reactions;
    unique_ptr<SymbolTable<K, V>> table_ptr;
    Vessel(string s = "") : title(s) {
      table_ptr = unique_ptr<SymbolTable<K, V>>(new SymbolTable<K, V>());
    }
    int environment() {return 0; } //idk what to do with this
    
    Reaction<K,V> add(K a, V b) {
      table_ptr->store(a, b);
      auto r = Reaction<K, V>{a,b};
      return r;
    }
    void add(const Reaction<K,V>&& r) {
      reactions.push_back(r);
    }
    void print_table(){
      table_ptr->print();
    }
    void do_reaction(const Reaction<K,V>& r) {
      for (auto& i : r.input){
        table_ptr->decrement(i);
        cout << "[Input] " << i << " " << table_ptr->lookup(i) << endl;
      }
      for (auto& i : r.product){
        table_ptr->increment(i);
        cout << "[Product] " << i << " " << table_ptr->lookup(i) << endl;
      }
    }
  };
  // Vessel(string,float) -> Vessel<string,float>; // deductor for compiler, might not be needed
}



