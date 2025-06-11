#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>

using namespace std;

template<typename K, typename V>
class SymbolTable {
  map<K, V> table;

public:
  SymbolTable() {}
  SymbolTable(const K& k, const V& v) { store(k, v); }
  SymbolTable(const map<K, V>& t) : table(t) {}
  SymbolTable(const SymbolTable<K, V>& st) : table(st.table) {}
  ~SymbolTable() {}

  void print() {
    for (const auto& [key, val] : table) {
      cout << key << " " << val << endl;
    }
  }

  void increment(K key) {
    table.at(key)++;
  }

  void decrement(K key) {
    table.at(key)--;
  }

  V lookup(const K& key) const {
    try {
      return table.at(key);
    } catch (const out_of_range& e) {
      cout << "Exception caught: key does not exist - ";
      throw;
    }
  }

  bool store(const K& key, const V& val) {
    try {
      if (table.find(key) == table.end()) {
        table.insert({key, val});
        return true;
      } else {
        string s = "key already exist";
        throw s;
      }
    } catch (const string& s) {
      cout << "Exception caught: " << s << endl;
      return false;
    }
  }

  void save_state(float timestep) const {
    const string filename = "../data.csv";
    bool file_exists = filesystem::exists(filename);

    ofstream file(filename, ios::app);
    if (!file.is_open()) {
      cerr << "Failed to open file\n";
      return;
    }

    if (!file_exists) {
      file << "time";
      for (const auto& [key, value] : table) {
        file << "," << key;
      }
      file << "\n";
    }

    file << timestep;
    for (const auto& [key, value] : table) {
      file << "," << value;
    }
    file << "\n";

    file.close();
  }
};

