#include <cstdlib>
#include <vessel.hpp>

using namespace std;


int main (int argc, char *argv[]) {
  // auto s = SymbolTable<string,int>();
  // s.store("A",0);
  // s.store("DA",1);
  // s.store("D_A",0);
  auto v = stocasthic::Vessel();
  v.environment();
  auto A = v.add("A", 0);
  auto DA = v.add("DA", 1);
  auto D_A = v.add("D_A", 0);
  float gammaA = 1;

  // auto gammaA = 1;
  // auto t = ((A + DA));
  // auto t = ((A + DA) >> gammaA);
  auto t = ((A + DA) >> gammaA >>= D_A);

  // cout << t << endl;


  return 0;
}


