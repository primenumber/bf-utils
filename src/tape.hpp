#pragma once
#include <vector>
#include <stdexcept>

class Tape {
 public:
  Tape() : data(4096), itr(std::begin(data)) {};
  Tape(const Tape &tape) : data(tape.data),
      itr(std::begin(data) + (tape.itr - std::begin(tape.data))) {}
  Tape(Tape &&) = default;
  Tape &operator=(const Tape &tape) {
    data = tape.data;
    itr = std::begin(data) + (tape.itr - std::begin(tape.data));
    return *this;
  }
  Tape &operator=(Tape &&) = default;
  void set(uint8_t val) { *itr = val; }
  void inc() { ++*itr; }
  void dec() { --*itr; }
  uint8_t get() const { return *itr; }
  void next() {
    ++itr;
    if (itr == std::end(data)) data.resize(data.size() * 2);
  }
  void prev() {
    if (itr == std::begin(data)) throw std::out_of_range("Tape::prev()");
    --itr;
  }
 private:
  std::vector<uint8_t> data;
  std::vector<uint8_t>::iterator itr;
};
