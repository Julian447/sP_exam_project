#include <algorithm>
#include <vessel.hpp>

#include <chrono>
#include <thread>

using namespace std;

auto circadian_rhythm() {
  const auto alphaA = 50;
  const auto alpha_A = 500;
  const auto alphaR = 0.01;
  const auto alpha_R = 50;
  const auto betaA = 50;
  const auto betaR = 5;
  const auto gammaA = 1;
  const auto gammaR = 1;
  const auto gammaC = 2;
  const auto deltaA = 1;
  const auto deltaR = 0.2;
  const auto deltaMA = 10;
  const auto deltaMR = 0.5;
  const auto thetaA = 50;
  const auto thetaR = 100;
  auto v = stochastic::Vessel{"Circadian Rhythm"};
  const auto env = v.environment();
  const auto DA = v.add("DA", 1);
  const auto D_A = v.add("D_A", 0);
  const auto DR = v.add("DR", 1);
  const auto D_R = v.add("D_R", 0);
  const auto MA = v.add("MA", 0);
  const auto MR = v.add("MR", 0);
  const auto A = v.add("A", 0);
  const auto R = v.add("R", 0);
  const auto C = v.add("C", 0);
  v.add((A + DA) >> gammaA >>= D_A);
  v.add(D_A >> thetaA >>= DA + A);
  v.add((A + DR) >> gammaR >>= D_R);
  v.add(D_R >> thetaR >>= DR + A);
  v.add(D_A >> alpha_A >>= MA + D_A);
  v.add(DA >> alphaA >>= MA + DA);
  v.add(D_R >> alpha_R >>= MR + D_R);
  v.add(DR >> alphaR >>= MR + DR);
  v.add(MA >> betaA >>= MA + A);
  v.add(MR >> betaR >>= MR + R);
  v.add((A + R) >> gammaC >>= C);
  v.add(C >> deltaA >>= R);
  // v.add(A >> deltaA >>= env);
  // v.add(R >> deltaR >>= env);
  // v.add(MA >> deltaMA >>= env);
  // v.add(MR >> deltaMR >>= env);
  return v;
}

auto simple_example() {
  auto v = stochastic::Vessel{};
  const auto A = v.add("A", 100);
  auto B = v.add("B", 0);
  auto C = v.add("C", 1);
  float gammaA = 0.001;
  v.add((A + C) >> gammaA >>= B + C);

  return v;
}

template<typename K = string, typename V = float>
void simulation(auto& v, const auto end_time) { // dunno a way to avoid auto for vessel parameter
  auto t = 0;
  while (t <= end_time) {
    //implement r.delay
    for (Reaction<K,V>& r : v.reactions) {
      map<K,V> m;
      for (auto key : r.input)
        m.insert({key,v.table_ptr->lookup(key)});

      r.calculate_delay(m);
    }
    auto it = std::min_element(v.reactions.begin(), v.reactions.end(),
                             [](const Reaction<K, V>& a, const Reaction<K, V>& b) {
                               return a.delay < b.delay;
                             });

    if (it != v.reactions.end()) {
      const auto& r = *it;
      t += r.delay;
      cout << "[Time] " << t << endl;
      cout << "[Delay] " << r.delay << endl;
      cout << "[Lambda] " << r.lambda << endl;
      if (all_of(r.input.begin(), r.input.end(), [&](const K key){ return v.table_ptr->lookup(key) >= 0; })) {
        v.do_reaction(r);
      }
      cout << endl;
      //store state
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
}


int main (int argc, char *argv[]) {

  // auto v = simple_example();
  auto v = circadian_rhythm();
  simulation(v, 2000);
  cout << endl;
  v.print_table();

  return 0;
}


