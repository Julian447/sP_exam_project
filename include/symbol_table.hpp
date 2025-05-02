#include <algorithm>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

template<typename K, typename V>
class SymbolTable {
  map<K,V> table;

  public:
    SymbolTable(){}
    SymbolTable(const K& k, const V& v){
      store(k,v);
    }
    SymbolTable(const map<K, V>& t) : table(t) {}
    SymbolTable(const SymbolTable<K,V>& st) : table(st.table) {}
    ~SymbolTable(){}
    
    V lookup(const K& key) const {
      try { 
        return table.at(key);
      } catch (const out_of_range& e) { // catch by const reference
        cout << "Exception caught: key does not exist - ";
        throw;
      }
    }

    bool store(const K& key, const V& val) {
      try { 
        if (table.find(key) == table.end()) {
          table.insert({key,val});
          cout << "Stored object in table" << endl;
          return true;
        }
        else {
          string s = "key already exist";
          throw s;
        }
      } catch (const string& s) { // catch by const reference
        cout << "Exception caught: " << s << endl;
        return false;
      }
    }
};

