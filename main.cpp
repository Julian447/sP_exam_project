#include <algorithm>
#include <future>
#include <vessel.hpp>
#include <semaphore>

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
void simulation(stochastic::Vessel<K,V>& v, const double end_time,
                auto&& observer = nullptr) {
  double t = 0;
  observer(*v.table_ptr, t);

  while (t <= end_time) {
    for (Reaction<K,V>& r : v.reactions) {
      map<K,V> m = {};
      for (auto key : r.input)
        m.insert({key,v.table_ptr->lookup(key)});

      r.calculate_delay(m);
    }

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

      if (all_of(r.input.begin(), r.input.end(), [&](const K key){ return v.table_ptr->lookup(key) > 0; })) {
        v.do_reaction(r);
      }

      //store state
      observer(*v.table_ptr, t);

      t += r.delay;
    } else {
      // std::cout << "Simulation " << t << " stopped early: no valid reactions left." << std::endl;
      break;
    }
  }
}

template<typename K = string, typename V = float>
void run_multiple_simulations(stochastic::Vessel<K, V>& v, const double end_time, 
                              size_t num_simulations, size_t max_concurrent, auto&& observer = nullptr) {
  std::vector<std::pair<double, double>> results(num_simulations); // Store results directly
  std::atomic<size_t> completed_runs = 0;

  auto worker = [&](size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
      auto v_copy = v; // Create a new copy for each simulation
      double peak_hospitalized = -1.0;
      double peak_time = -1.0;

      auto peak_tracker = [&](const SymbolTable<K, V>& table, double t) {
        float current = table.lookup("H");
        if (current > peak_hospitalized) {
          peak_hospitalized = current;
          peak_time = t;
        }
      };

      simulation(v_copy, end_time, peak_tracker);
      ++completed_runs;
      std::cout << "\rCompleted simulations: " << completed_runs << " / " << num_simulations << std::flush;
      results[i] = std::make_pair(peak_hospitalized, peak_time); // Store results directly
    }
  };

  size_t batch_size = (num_simulations + max_concurrent - 1) / max_concurrent;
  std::vector<std::thread> threads;

  for (size_t i = 0; i < max_concurrent; ++i) {
    size_t start = i * batch_size;
    size_t end = std::min(start + batch_size, num_simulations);
    if (start < end) {
      threads.emplace_back(worker, start, end);
    }
  }

  for (auto& thread : threads) {
    thread.join();
  }

  double total_peak_hospitalized = 0.0;
  double total_peak_time = 0.0;

  for (const auto& result : results) {
    total_peak_hospitalized += result.first;
    total_peak_time += result.second;
  }

  double avg_peak_hospitalized = total_peak_hospitalized / num_simulations;
  double avg_peak_time = total_peak_time / num_simulations;

  std::cout << "\nAverage peak hospitalized: " << avg_peak_hospitalized 
            << " at average time: " << avg_peak_time << std::endl;
}


void benchmark_simulation(auto& v, const double end_time, auto&& observer, int N_multi, int max_concurrent) {
  using namespace std::chrono;

  std::cout << "\nBenchmarking simulations...\n";
  auto start_multi = high_resolution_clock::now();
  run_multiple_simulations(v, end_time, N_multi, max_concurrent, observer);
  auto end_multi = high_resolution_clock::now();
  auto duration_multi = duration_cast<milliseconds>(end_multi - start_multi).count();
  std::cout << "\rSimulations completed in " << duration_multi << " ms.\n";
}


int main (int argc, char *argv[]) {
  remove_data();

  auto start = chrono::high_resolution_clock::now();
  auto N = 100;

  auto full_logger = [](const SymbolTable<std::string, float>& table, double t) {
    table.save_state(t);
    cout << "\rProgress: " << fixed << setprecision(2) << t << flush;
  };

  auto peak_hospitalized = -1.0;
  auto peak_time = -1.0;
  auto peak_tracker = [&](const SymbolTable<std::string, float>& table, double t) {
    float current = table.lookup("H");
    if (current > peak_hospitalized) {
      peak_hospitalized = current;
      peak_time = t;
    }
    // cout << "\rProgress: " << fixed << setprecision(2) << t << flush;
  };

  auto N_base = 10'000;
  auto N_NJ = 590'000;
  auto N_DK = 5'947'000;

  // auto v = simple_example();
  // auto v = circadian_rhythm();
  auto v = seihr(N_base);
  // auto v = seihr(N_NJ);
  // auto v = seihr(N_DK);
  // simulation(v, N, full_logger);
  // simulation(v_cpy, N, peak_tracker);

  benchmark_simulation(v, N, peak_tracker, 5, 1); 
  benchmark_simulation(v, N, peak_tracker, 5, 2); 
  benchmark_simulation(v, N, peak_tracker, 5, 4);
  // benchmark_simulation(v, N, peak_tracker, 5, 8); 
 
  // benchmark_simulation(v, N, peak_tracker, 100, 1);
  // benchmark_simulation(v, N, peak_tracker, 100, 2);
  // benchmark_simulation(v, N, peak_tracker, 100, 6);
  // benchmark_simulation(v, N, peak_tracker, 100, 8);
  // benchmark_simulation(v, N, peak_tracker, 100, 12);

  cout << endl;

  if (peak_time != -1.0 && peak_hospitalized != -1.0)
    cout << "Peak hospitalized: " << peak_hospitalized << " at time " << peak_time << endl;

  auto end = chrono::high_resolution_clock::now();

  chrono::duration<double> duration = end - start;

  cout << "Total execution time: " << std::fixed << setprecision(4) << duration.count() << " seconds" << endl;

  return 0;
}


