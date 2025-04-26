#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class HandleFile {
  string str; 
  // vector<string> input;
  string input;

  public:
    void get_file();
    void part1();
    void part2();
    void print();
};

