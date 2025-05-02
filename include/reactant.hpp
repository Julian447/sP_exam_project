#include <string>
#include <memory>

#include <symbol_table.hpp>
#include <utility>

template<typename K, typename V>
class Reactant {
  public:
    shared_ptr<SymbolTable<string, int>> table_ptr;
    K key;
    V val;
    Reactant(K k, V v, shared_ptr<SymbolTable<string, int>> tp) : key(k), val(v), table_ptr(tp) {}
    // Reactant operator+(Reactant const& b) {
    //   cout << this->key << " " << this->val-1 << endl;
    //   cout << b.key << " " << b.val-1 << endl;
    //   return *this;
    // }
    // bool operator>>(float b){
    //   cout << b << endl;
    //   return true;
    // }
    // operator>>=
    // friend Reactant operator+(Reactant const& lhs, Reactant const& rhs);
    // friend bool operator>>(Reactant const& lhs, float rhs);
    // friend Reactant operator>>=(bool lhs, Reactant const& rhs);

  friend pair<Reactant, Reactant> operator+(Reactant const& lhs, Reactant const& rhs) {
    cout << lhs.key << " " << lhs.val-1 << endl;
    cout << rhs.key << " " << rhs.val-1 << endl;
    return {lhs, rhs};
  }
  friend bool operator>>(pair<Reactant, Reactant> const& lhs, float rhs) {
    cout << rhs << endl;
    return true;
  }
  friend Reactant operator>>=(bool lhs, Reactant const& rhs) { 
    cout << rhs.key << " " << rhs.val+1 << endl;
    return rhs;
  }
};





