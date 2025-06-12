#include <memory>
#include <string>
#include <vector>

#include <reaction.hpp>


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

  void do_reaction(const Reaction<K, V>& r) {
    for (const auto& i : r.input) {
      table_ptr->decrement(i);
    }
    for (const auto& i : r.product) {
      table_ptr->increment(i);
    }
  }

  void print_table() {
    constexpr int dash_count = 10;
    string dashes(dash_count, '-');

    cout << dashes << "Start Table" << dashes << endl;
    table_ptr->print();
    cout << dashes << "Reactions" << dashes << endl;
    for (const Reaction<K,V>& r : reactions) {
      r.print_reaction();
    }
    cout << dashes << "End Table" << dashes << endl;
    table_ptr->print();
  }
};
