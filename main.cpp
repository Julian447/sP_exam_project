#include <algorithm>
#include <vessel.hpp>

#include <chrono>
#include <thread>

using namespace std;
namespace fs = filesystem;

void remove_data() {
  const string filename = "../data.csv";

  if (fs::exists(filename)) {
      try {
          fs::remove(filename);
          std::cout << "File '" << filename << "' removed successfully.\n";
      } catch (const fs::filesystem_error& e) {
          std::cerr << "Error removing file: " << e.what() << '\n';
      }
  } else {
      std::cout << "File '" << filename << "' does not exist, nothing removed.\n";
  }
}

auto seihr(uint32_t N) {
  auto v = stochastic::Vessel{"COVID19 SEIHR: " + std::to_string(N)};
  const auto eps = 0.0009; // initial fraction of infectious
  const auto I0 = size_t(std::round(eps * N)); // initial infectious
  const auto E0 = size_t(std::round(eps * N * 15)); // initial exposed
  const auto S0 = N - I0 - E0; // initial susceptible
  const auto R0 = 2.4; // initial basic reproductive number
  const auto alpha = 1.0 / 5.1; // incubation rate (E -> I) ~5.1 days
  const auto gamma = 1.0 / 3.1; // recovery rate (I -> R) ~3.1 days
  const auto beta = R0 * gamma; // infection/generation rate (S+I -> E+I)
  const auto P_H = 0.9e-3; // probability of hospitalization
  const auto kappa = gamma * P_H * (1.0 - P_H); // hospitalization rate (I -> H)
  const auto tau = 1.0 / 10.12; // removal rate in hospital (H -> R) ⤦
  const auto S = v.add("S", S0); // susceptible
  const auto E = v.add("E", E0); // exposed
  const auto I = v.add("I", I0); // infectious
  const auto H = v.add("H", 0); // hospitalized
  const auto R = v.add("R", 0); // removed/immune (recovered + dead)
  v.add((S + I) >> beta / N >>= E + I); // susceptible becomes exposed by infectious
  v.add(E >> alpha >>= I); // exposed becomes infectious
  v.add(I >> gamma >>= R); // infectious becomes removed
  v.add(I >> kappa >>= H); // infectious becomes hospitalized
  v.add(H >> tau >>= R); // hospitalized becomes removed
  return v;
}

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
  v.add(A >> deltaA >>= env);
  v.add(R >> deltaR >>= env);
  v.add(MA >> deltaMA >>= env);
  v.add(MR >> deltaMR >>= env);
  return v;
}

auto simple_example() {
  auto v = stochastic::Vessel{};
  const auto A = v.add("A", 50);
  auto B = v.add("B", 50);
  auto C = v.add("C", 1);
  float gammaA = 0.001;
  v.add((A + C) >> gammaA >>= B + C);

  return v;
}

template<typename K = string, typename V = float>
void simulation(auto& v, const auto end_time) { // dunno a way to avoid auto for vessel parameter
  double t = 0;
  v.table_ptr->save_state(t);
  while (t <= end_time) {
    for (Reaction<K,V>& r : v.reactions) {
      map<K,V> m = {};
      for (auto key : r.input)
        m.insert({key,v.table_ptr->lookup(key)});

      r.calculate_delay(m);
    }

    // auto it = std::min_element(v.reactions.begin(), v.reactions.end(),
    //                          [](const Reaction<K, V>& a, const Reaction<K, V>& b) {
    //                            return a.delay < b.delay;
    //                          });

    auto it = v.reactions.end();
    for (auto iter = v.reactions.begin(); iter != v.reactions.end(); ++iter) {
      if (iter->delay <= 0.0) continue;
      if (it == v.reactions.end() || iter->delay < it->delay) {
          it = iter;
      }
    }

    if (it != v.reactions.end()) {
      const auto& r = *it;
      // r.print_reaction();

      t += r.delay;
      // cout << "[Time] " << t << endl;
      // cout << "[Delay] " << r.delay << endl;
      // cout << "[Lambda] " << r.lambda << endl;
      if (all_of(r.input.begin(), r.input.end(), [&](const K key){ return v.table_ptr->lookup(key) > 0; })) {
        v.do_reaction(r);
      }
      // cout << "hit" << t << endl;
      //store state
      v.table_ptr->save_state(t);


      // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
      // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
}




int main (int argc, char *argv[]) {
  remove_data();


  // auto v = simple_example();
  auto v = circadian_rhythm();
  // auto v = seihr(10000);
  // simulation(v, 100);
  simulation(v, 48);
  cout << endl;
  v.print_table();

  return 0;
}


