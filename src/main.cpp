#include <iostream>
#include <fstream>
#include "prog.hpp"

int main(int argc, char **argv) {
  std::ifstream ifs(argv[1]);
  if (!ifs) {
    std::cerr << "cannot open file: " << argv[1] << std::endl;
    return 1;
  }
  Program prog;
  prog.parse(ifs);
  prog.generate_cpp_code(std::cout);
  return 0;
}
