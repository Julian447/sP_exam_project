#include <random>
#include <iostream>
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
    double lambda;
    vector<K> product;
    double delay = 0;
    Reaction() {}
    Reaction(K k, V v) : key(k), val(v) {}

  void calculate_delay(const map<K,V>& inputs) {
    if (inputs.empty()) {
      return;
    }

    double product = 1.0;
    // cout << "Inputs for reaction: ";
    for (auto const& pair : inputs) {
      // cout << key << ": " << val << ", ";
      // cout << key << " " << val << endl;
      product *= pair.second;
    }
    // cout << endl;

    double rate = lambda * product;
    // cout << "Lambda: " << lambda << ", Product: " << product << ", Rate: " << rate << endl;

    if (rate <= 0.0) {
      // cout << "Rate is non-positive, delay not calculated." << endl;
      delay = 0.0;
      return;
    }

    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<> distribution(rate);

    delay = distribution(gen);
    // cout << "Calculated delay: " << delay << endl;

    // Debugging output
    // cout << "[Reaction] Inputs: ";
    // for (auto const& [key, val] : inputs) {
    //     cout << key << "=" << val << " ";
    // }
    cout << "[Reaction] Lambda: " << lambda << " | Product: " << product << " | Rate: " << rate << " | Delay: " << delay << endl;
}

  friend pair<Reaction, Reaction> operator+(Reaction const& lhs, Reaction const& rhs) {
    return make_pair(lhs, rhs);
  }
  friend Reaction operator>>(const Reaction& lhs, const float rhs) {
    Reaction r = lhs;

    r.lambda = rhs;

    r.insertKeys(lhs);

    return r;
  }
  friend Reaction operator>>(pair<Reaction, Reaction> lhs, const float rhs) {
    auto fst = lhs.first;
    auto snd = lhs.second;

    fst.lambda = rhs;

    fst.insertKeys(lhs.first);
    fst.insertKeys(lhs.second);

    return fst;
  }
  friend Reaction operator>>=(Reaction lhs, Reaction const& rhs) { 
    Reaction r = lhs;
    r.insertProductKeys(rhs);
    return r;
  }
  friend Reaction operator>>=(Reaction lhs, pair<Reaction, Reaction> rhs) { 
    Reaction r = lhs;
    r.insertProductKeys(rhs.first);
    r.insertProductKeys(rhs.second);

    return r;
  }
  friend Reaction operator>>=(Reaction lhs, const string& rhs) { 
    // Reaction r = lhs;
    // r.insertProductKeys(rhs);

    return lhs;
  }

  void insertKeys(const Reaction& other) {
    // Insert other's key into this->input if not already there
    if (std::find(input.begin(), input.end(), other.key) == input.end()) {
      input.push_back(other.key);
    }
  }

  void insertProductKeys(const Reaction& other) {
    // Insert other's key into this->product if not already there
    if (std::find(product.begin(), product.end(), other.key) == product.end()) {
      product.push_back(other.key);
    }
  }

  void print_reaction() const {
    std::cout << "(";
    for (size_t i = 0; i < input.size(); ++i) {
      std::cout << input[i];
      if (i < input.size() - 1) {
          std::cout << " + ";
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

