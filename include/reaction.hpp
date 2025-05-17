#include <random>
#include <set>
#include <utility>

#include <symbol_table.hpp>
#include <vector>

using namespace std;

template<typename K, typename V>
class Reaction {
  K key;
  V val;
  public:
    vector<K> input;
    float lambda;
    vector<K> product;
    double delay;
    Reaction() {}
    Reaction(K k, V v) : key(k), val(v) {}

  void calculate_delay(const map<K,V>& inputs) {
    if (inputs.empty()) {
        return; 
    }

    double product = 1.0;
    for (auto const& [key,val] : inputs) {
      product *= static_cast<double>(val);
      cout << key << endl;
    }
    double rate = lambda * product;

    if (rate <= 0.0) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> distribution(rate);

    delay = distribution(gen);
  }

  friend pair<Reaction, Reaction> operator+(Reaction const& lhs, Reaction const& rhs) {
    return make_pair(lhs, rhs);
  }
  friend pair<Reaction,bool> operator>>(const Reaction& lhs, const float rhs) {
    Reaction r = lhs;
    if (find(r.input.begin(), r.input.end(), r.key) == r.input.end())
      r.input.push_back(r.key);

    return make_pair(r, true);
  }
  friend pair<Reaction,bool> operator>>(pair<Reaction, Reaction> lhs, const float rhs) {
    auto fst = lhs.first;
    auto snd = lhs.second;
    if (find(fst.input.begin(), fst.input.end(), fst.key) == fst.input.end())
      fst.input.push_back(fst.key);
    if (find(fst.input.begin(), fst.input.end(), snd.key) == fst.input.end())
      fst.input.push_back(snd.key);

    fst.lambda = rhs;

    fst.insertKeys(snd);

    return make_pair(fst, true);
  }
  friend Reaction operator>>=(pair<Reaction,bool> lhs, Reaction const& rhs) { 
    Reaction r = lhs.first;
    if (find(r.product.begin(), r.product.end(), rhs.key) == r.product.end())
      r.product.push_back(rhs.key);

    r.insertKeys(rhs);

    return r;
  }
  friend Reaction operator>>=(pair<Reaction, bool> lhs, pair<Reaction, Reaction> rhs) { 
    Reaction r = lhs.first;
    if (find(r.product.begin(), r.product.end(), rhs.first.key) == r.product.end())
      r.product.push_back(rhs.first.key);
    if (find(r.product.begin(), r.product.end(), rhs.second.key) == r.product.end())
      r.product.push_back(rhs.second.key);

    r.insertKeys(rhs.first);
    r.insertKeys(rhs.second);

    return r;
  }

  void insertKeys(Reaction const& other){

    std::set<string> inputSet(input.begin(), input.end());
    inputSet.insert(other.input.begin(), other.input.end());
    input.assign(inputSet.begin(), inputSet.end());

    std::set<string> productSet(product.begin(), product.end());
    productSet.insert(other.product.begin(), other.product.end());
    product.assign(productSet.begin(), productSet.end());
  }
};

