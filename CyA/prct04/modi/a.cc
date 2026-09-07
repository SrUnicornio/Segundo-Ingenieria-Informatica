/**
 * Codigo ejemplo para la práctica 4 de CyA
 */
#include <iostream>
#include <string>

int main() {
  int a = 5, b, c = 10;
  double x = 3.14, y{2.71}, z;
  for (int i = 0, j{1}; i < a; ++i) {
    int alga = i * 2;
    if (i % 2 == 0) {
      std::cout << "Even: " << i << std::endl;
    } else {
      std::cout << "Odd: " << i << std::endl;
    }
  }
  for (int i = 0, j{1}; i < 2; ++i) {
  }
  int a, b{2}, c = 3;
  do {
    std::cout << "Looping..." << std::endl;
  } while (false);
  while (true) {
    break;
  }
  double pi = 3.14159;
  do {
    std::cout << "Pi is approximately " << pi << std::endl;
  } while (false);
  std::string message = "Hello, World!";
  std::cout << "Hello, World!" << std::endl;
  if (a > b) {
    std::cout << "a is greater than b" << std::endl;
  } else if (a < b) {
    std::cout << "a is less than b" << std::endl;
  } else {
    std::cout << "a is equal to b" << std::endl;
  }

  switch (a) {
    case 1:
      std::cout << "a is one" << std::endl;
      break;
    case 2:
      std::cout << "a is two" << std::endl;
      break;
    default:
      std::cout << "a is something else" << std::endl;
      break;
  }
  return 0;
}