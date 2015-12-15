#pragma once
#include "tape.hpp"
#include <iostream>

class Program {
 public:
  Program() = default;
  void parse(std::istream &);
  void run(std::istream &, std::ostream &);
  void generate_cpp_code(std::ostream &);
 private:
  int parse_impl(std::istream &, bool);
  int push_block(std::vector<int> &&);
  void run_impl(int, Tape &, std::istream &, std::ostream &);
  std::vector<std::vector<int>> commands;
  int entry_point;
};
