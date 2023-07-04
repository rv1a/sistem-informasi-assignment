#include "Utils.hpp"
#include <iostream>
#include <limits>

namespace Utils {
void reset_cin_state() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
} // namespace Utils
