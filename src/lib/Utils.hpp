#ifndef SRC_LIB_UTILS
#define SRC_LIB_UTILS

#include <iostream>
#include <istream>
#include <string>
#include <type_traits>

namespace Utils {
// Untuk mereset cin supaya tidak stuck pada saat user input.
void reset_cin_state();

// Type trait supaya fungsi prompt hanya menerima tipe data tertentu.
template <typename T>
constexpr bool is_promptable =
    std::disjunction<std::is_arithmetic<T>,
                     std::is_same<T, std::string>>::value;

// Fungsi helper untuk menampilkan prompt dan menerima input dari user.
template <typename T> T prompt(std::string message) {
  static_assert(is_promptable<T>, "Tipe data tidak disupport");
  T input;
  std::cout << message;
  if constexpr (std::is_same_v<T, std::string>) {
    std::getline(std::cin >> std::ws, input);
  } else {
    std::cin >> input;
  }
  return input;
};

} // namespace Utils

#endif /* SRC_LIB_UTILS */
