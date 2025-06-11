#include <memory>
#include <mutex>
#include <string>

// #include <symbol_table.hpp>
#include <reaction.hpp>
#include <vector>


using namespace std;

template<typename K = string, typename V = float>
class Vessel {
  string title;

public:
  vector<Reaction<K, V>> reactions;
  unique_ptr<SymbolTable<K, V>> table_ptr;

  Vessel(string s = "") : title(s) {
    table_ptr = make_unique<SymbolTable<K, V>>();
  }

  // Copy constructor
  Vessel(const Vessel& other)
    : title(other.title), reactions(other.reactions) {
    table_ptr = make_unique<SymbolTable<K, V>>(*other.table_ptr);
  }

  string environment() { return "env"; }

  Reaction<K, V> add(K a, V b) {
    table_ptr->store(a, b);
    return Reaction<K, V>{a, b};
  }

  void add(const Reaction<K, V>&& r) {
    reactions.push_back(r);
  }

  void print_table() {
    table_ptr->print();
  }

  void do_reaction(const Reaction<K, V>& r) {
    for (const auto& i : r.input) {
      table_ptr->decrement(i);
    }
    for (const auto& i : r.product) {
      table_ptr->increment(i);
    }
  }

  void do_reaction_concurrent(const Reaction<K, V>& r, mutex& external_mutex) {
    lock_guard<mutex> lock(external_mutex);
    for (const auto& i : r.input) {
      table_ptr->decrement(i);
    }
    for (const auto& i : r.product) {
      table_ptr->increment(i);
    }
  }
};


