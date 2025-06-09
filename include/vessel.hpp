#include <memory>
#include <mutex>
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
    // mutex simulation_mutex;
    vector<Reaction<K,V>> reactions;
    unique_ptr<SymbolTable<K, V>> table_ptr;

    Vessel(string s = "") : title(s) {
      table_ptr = unique_ptr<SymbolTable<K, V>>(new SymbolTable<K, V>());
    }

    // Copy constructor
    Vessel(const Vessel& other) : title(other.title), reactions(other.reactions) {
      table_ptr = unique_ptr<SymbolTable<K, V>>(new SymbolTable<K, V>(*other.table_ptr));
    }

    string environment() {return "env"; } //idk what to do with this
    
    Reaction<K,V> add(K a, V b) {
      // lock_guard<mutex> lock(simulation_mutex);
      table_ptr->store(a, b);
      auto r = Reaction<K, V>{a,b};
      return r;
    }
    void add(const Reaction<K,V>&& r) {
      reactions.push_back(r);
    }
    void print_table(){
      // lock_guard<mutex> lock(simulation_mutex);
      table_ptr->print();
    }
    void do_reaction(const Reaction<K,V>& r) {
      // lock_guard<mutex> lock(simulation_mutex);
      for (auto& i : r.input){
        table_ptr->decrement(i);
        // cout << "[Input] " << i << " " << table_ptr->lookup(i) << endl;
      }
      for (auto& i : r.product){
        table_ptr->increment(i);
        // cout << "[Product] " << i << " " << table_ptr->lookup(i) << endl;
      }
    }
    void do_reaction_concurrent(const Reaction<K,V>& r, mutex& external_mutex) {
      lock_guard<mutex> lock(external_mutex);
      for (auto& i : r.input){
        table_ptr->decrement(i);
        // cout << "[Input] " << i << " " << table_ptr->lookup(i) << endl;
      }
      for (auto& i : r.product){
        table_ptr->increment(i);
        // cout << "[Product] " << i << " " << table_ptr->lookup(i) << endl;
      }
    }
  };
}



