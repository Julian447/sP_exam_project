#include <random>
#include <iostream>
#include <set>
#include <utility>
#include <algorithm>

#include <symbol_table.hpp>
#include <vector>

using namespace std;

template<typename K, typename V>
class Reaction {
  K key;
  V val;

public:
  vector<K> input;
  double lambda;
  vector<K> product;
  double delay = 0;

  Reaction() {}
  Reaction(const K& k, const V& v) : key(k), val(v) {}

  K get_key() const { return key; }

  void calculate_delay(const map<K, V>& inputs) {
    if (inputs.empty()) {
      return;
    }

    double product_val = 1.0;
    for (const auto& pair : inputs) {
      product_val *= pair.second;
    }

    double rate = lambda * product_val;

    if (rate <= 0.0) {
      delay = 0.0;
      return;
    }

    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<> distribution(rate);

    delay = distribution(gen);
  }

  friend pair<Reaction, Reaction> operator+(const Reaction& lhs, const Reaction& rhs) {
    return make_pair(lhs, rhs);
  }

  friend Reaction operator>>(Reaction lhs, const float rhs) {
    lhs.lambda = rhs;
    lhs.insertKeys(lhs);
    return lhs;
  }

  friend Reaction operator>>(const pair<Reaction, Reaction>& lhs, const float rhs) {
    Reaction fst = lhs.first;
    fst.lambda = rhs;
    fst.insertKeys(lhs.first);
    fst.insertKeys(lhs.second);
    return fst;
  }

  friend Reaction operator>>=(Reaction lhs, const Reaction& rhs) {
    lhs.insertProductKeys(rhs);
    return lhs;
  }

  friend Reaction operator>>=(Reaction lhs, const pair<Reaction, Reaction>& rhs) {
    lhs.insertProductKeys(rhs.first);
    lhs.insertProductKeys(rhs.second);
    return lhs;
  }

  friend Reaction operator>>=(Reaction lhs, const string& rhs) {
    return lhs;
  }

  void insertKeys(const Reaction& other) {
    if (find(input.begin(), input.end(), other.key) == input.end()) {
      input.push_back(other.key);
    }
  }

  void insertProductKeys(const Reaction& other) {
    if (find(product.begin(), product.end(), other.key) == product.end()) {
      product.push_back(other.key);
    }
  }

  void print_reaction() const {
    cout << "(";
    for (size_t i = 0; i < input.size(); ++i) {
      cout << input[i];
      if (i < input.size() - 1) {
        cout << " + ";
      }
    }
    cout << ") --" << lambda << "--> (";
    for (size_t i = 0; i < product.size(); ++i) {
      cout << product[i];
      if (i < product.size() - 1) {
        cout << " + ";
      }
    }
    cout << ")" << endl;
  }
};

