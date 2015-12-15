#include "prog.hpp"

enum Command {
  INCREMENT = -1,
  DECREMENT = -2,
  NEXT = -3,
  PREV = -4,
  OUTPUT = -5,
  INPUT = -6
};

void Program::parse(std::istream &prog) {
  entry_point = parse_impl(prog, true);
}

int Program::parse_impl(std::istream &prog, bool is_root) {
  std::vector<int> block;
  while (!prog.eof()) {
    char command;
    prog >> command;
    if (prog.eof()) break;
    switch (command) {
      case '[':
        block.push_back(parse_impl(prog, false));
        break;
      case ']':
        if (is_root) throw std::runtime_error("parse error");
        else return push_block(std::move(block));
      case '+': block.push_back(Command::INCREMENT); break;
      case '-': block.push_back(Command::DECREMENT); break;
      case '>': block.push_back(Command::NEXT); break;
      case '<': block.push_back(Command::PREV); break;
      case '.': block.push_back(Command::OUTPUT); break;
      case ',': block.push_back(Command::INPUT); break;
      default:;
    }
  }
  if (is_root) return push_block(std::move(block));
  else throw std::runtime_error("parse error");
}

int Program::push_block(std::vector<int> &&block) {
  for (int i = 0; i < commands.size(); ++i)
    if (commands[i] == block)
      return i;
  commands.push_back(block);
  return commands.size() - 1;
}

void Program::run(std::istream &input, std::ostream &output) {
  Tape tape;
  run_impl(entry_point, tape, input, output);
}

void Program::run_impl(int index, Tape &tape, std::istream &input, std::ostream &output) {
  for (int command : commands[index]) {
    switch (command) {
      case Command::INCREMENT: tape.inc(); break;
      case Command::DECREMENT: tape.dec(); break;
      case Command::NEXT: tape.next(); break;
      case Command::PREV: tape.prev(); break;
      case Command::OUTPUT: output << tape.get(); break;
      case Command::INPUT: {
        char ch;
        input >> ch;
        tape.set(ch);
      } break;
      default:
        while (tape.get()) run_impl(command, tape, input, output);
    }
  }
}

void Program::generate_cpp_code(std::ostream &output) {
  output << "#include <iostream>\n";
  output << "#include \"tape.hpp\"\n";
  for (int i = 0; i < commands.size(); ++i) {
    output << "void func" << i << "(Tape &tape) {\n";
    for (int command : commands[i]) {
      switch (command) {
        case Command::INCREMENT: output << "  tape.inc();\n"; break;
        case Command::DECREMENT: output << "  tape.dec();\n"; break;
        case Command::NEXT:      output << "  tape.next();\n"; break;
        case Command::PREV:      output << "  tape.prev();\n"; break;
        case Command::OUTPUT:    output << "  std::cout << tape.get();\n"; break;
        case Command::INPUT:
          output << "  {\n    char ch;\n    std::cin >> ch;\n    tape.set(ch);\n  }\n";
          break;
        default:
          output << "  while (tape.get()) func" << command << "(tape);\n";
      }
    }
    output << "}\n";
  }
  output << "int main() {\n";
  output << "  Tape tape;\n";
  output << "  func" << (commands.size() - 1) << "(tape);\n";
  output << "  return 0;\n";
  output << "}" << std::endl;
}
