#include <utility>

#include <symbol_table.hpp>
#include <vector>

using namespace std;

template<typename K, typename V>
class Reaction {
  K key;
  V val;
  public:
    vector<K> input_keys;
    vector<K> product_keys;
    Reaction() {}
    Reaction(K k, V v) : key(k), val(v) {}

  friend pair<Reaction, Reaction> operator+(Reaction const& lhs, Reaction const& rhs) {
    return make_pair(lhs, rhs);
  }
  friend pair<Reaction,bool> operator>>(const Reaction& lhs, const float rhs) {
    Reaction r = lhs;
    if (find(r.input_keys.begin(), r.input_keys.end(), r.key) == r.input_keys.end())
      r.input_keys.push_back(r.key);

    return make_pair(r, true);
  }
  friend pair<Reaction,bool> operator>>(pair<Reaction, Reaction> lhs, const float rhs) {
    auto fst = lhs.first;
    auto snd = lhs.second;
    if (find(fst.input_keys.begin(), fst.input_keys.end(), fst.key) == fst.input_keys.end())
      fst.input_keys.push_back(fst.key);
    if (find(fst.input_keys.begin(), fst.input_keys.end(), snd.key) == fst.input_keys.end())
      fst.input_keys.push_back(snd.key);

    fst.insertKeys(snd);

    return make_pair(fst, true);
  }
  friend Reaction operator>>=(pair<Reaction,bool> lhs, Reaction const& rhs) { 
    Reaction r = lhs.first;
    if (find(r.product_keys.begin(), r.product_keys.end(), rhs.key) == r.product_keys.end())
      r.product_keys.push_back(rhs.key);

    r.insertKeys(rhs);
    for (auto& i : r.input_keys)
      cout << i << endl;
    cout << endl;
    for (auto& i : r.product_keys)
      cout << i << endl;
    cout << endl;

    return r;
  }
  friend Reaction operator>>=(pair<Reaction, bool> lhs, pair<Reaction, Reaction> rhs) { 
    Reaction r = lhs.first;
    if (find(r.product_keys.begin(), r.product_keys.end(), rhs.first.key) == r.product_keys.end())
      r.product_keys.push_back(rhs.first.key);
    if (find(r.product_keys.begin(), r.product_keys.end(), rhs.second.key) == r.product_keys.end())
      r.product_keys.push_back(rhs.second.key);

    r.insertKeys(rhs.first);
    r.insertKeys(rhs.second);

    return r;
  }

  void insertKeys(Reaction const& other){

    input_keys.insert(input_keys.end(), other.input_keys.begin(), other.input_keys.end());
    product_keys.insert(product_keys.end(), other.product_keys.begin(), other.product_keys.end());
  }
};

